ALTER TABLE `OpenHAB`.`items`
ADD COLUMN `numrows` INT NOT NULL DEFAULT -1;

ALTER TABLE `OpenHAB`.`items`
ADD COLUMN `lastupdate` DATE DEFAULT NULL;

ALTER TABLE `OpenHAB`.`items`
ADD COLUMN `deletedouble` INT NOT NULL DEFAULT 0;

DELIMITER $$
CREATE OR REPLACE PROCEDURE `OpenHAB`.`DeleteDoubleValues`()
BEGIN
# declare variables
DECLARE var_table, var_itemname VARCHAR(500);
DECLARE var_tableId INT DEFAULT 0;
DECLARE var_done INT DEFAULT 0;
DECLARE var_limit INT DEFAULT 200;
# This builds an cursor on all Item-Tables
DECLARE myCursor CURSOR FOR SELECT ItemId, itemname FROM items WHERE deletedouble=1;
DECLARE CONTINUE HANDLER FOR NOT FOUND SET var_done = 1;
OPEN myCursor;

# go through all ItemIds and delete double and unneeded values (previousValue and next value are the same)
read_loop: LOOP
	FETCH myCursor INTO var_tableId, var_itemname;
    IF var_done THEN
      LEAVE read_loop;
    END IF;
    SET var_table = concat('Item', var_tableID);
	select concat('DEBUG - ', 'Squeeze table: ', var_table, ' - ', var_itemname) AS 'Debug:';
    SET @del_sql = concat('DELETE del FROM ', var_table, ' AS del JOIN (SELECT * FROM (((SELECT ROW_NUMBER() OVER(ORDER BY time DESC) AS rowA, time as timeA, value as valueA from ', var_table, ' LIMIT ', var_limit, ') as A JOIN (SELECT ROW_NUMBER() OVER(ORDER BY time DESC) AS rowB, value as valueB from ', var_table, ' LIMIT ', var_limit , ') as B ON A.rowA=B.rowB+1 ) JOIN (SELECT ROW_NUMBER() OVER(ORDER BY time DESC) AS rowC, value as valueC from ', var_table, ' LIMIT ', var_limit , ') as C ON A.rowA=C.rowC-1 ) where A.valueA=B.valueB and A.valueA=C.valueC) as indel ON del.time=indel.timeA');
    PREPARE psmt FROM @del_sql;
    EXECUTE psmt;
    DEALLOCATE PREPARE psmt;
END LOOP;

CLOSE myCursor;
END$$

SET GLOBAL event_scheduler = ON;
CREATE EVENT cleanup_OldItems ON SCHEDULE EVERY 1 HOUR STARTS '2020-11-10 23:55:00' DO call DeleteDoubleValues();

call DeleteDoubleValues();

DELIMITER $$
CREATE OR REPLACE PROCEDURE `OpenHAB`.`UpdateDBStats`()
BEGIN
# declare variables
DECLARE var_table, var_itemname VARCHAR(500);
DECLARE var_tableId INT DEFAULT 0;
DECLARE var_done INT DEFAULT 0;
# This builds an cursor on all Item-Tables
DECLARE myCursor CURSOR FOR SELECT ItemId, itemname FROM items where ItemId;
DECLARE CONTINUE HANDLER FOR NOT FOUND SET var_done = 1;
OPEN myCursor;

read_loop: LOOP
	FETCH myCursor INTO var_tableId, var_itemname;
    IF var_done THEN
      LEAVE read_loop;
    END IF;
    SET var_table = concat('Item', var_tableID);
	select concat('DEBUG - ', 'Update table: ', var_table, ' - ', var_itemname) AS 'Debug:';
    SET @count_sql = concat('update items set numrows = (select count(*) from ', var_table, ') where ItemId = ', var_tableId);
    PREPARE psmt FROM @count_sql;
    EXECUTE psmt;
    DEALLOCATE PREPARE psmt;
    SET @maxdate_sql = concat('update items set lastupdate = (select max(time) from ', var_table, ') where ItemId = ', var_tableId);
    PREPARE psmt FROM @maxdate_sql;
    EXECUTE psmt;
    DEALLOCATE PREPARE psmt;
END LOOP;

CLOSE myCursor;
END$$

call UpdateDBStats();

select *, numrowsmax - numrows as delta from items where itemname not like '%LastUpdate' order by delta;
