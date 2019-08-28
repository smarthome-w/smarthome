select TABLE_NAME, TABLE_ROWS, CREATE_TIME, UPDATE_TIME, items.itemname 
from information_schema.tables, items
where TABLE_NAME = concat('Item', items.itemid) and TABLE_SCHEMA = 'OpenHAB' and TABLE_ROWS = 0;


delimiter //

CREATE procedure OpenHAB.clear_old_tables()
wholeblock:BEGIN
  DECLARE x INT;
  DECLARE str VARCHAR(255);
  SET x = -5;
  SET str = '';

  loop_label: LOOP
    IF x > 0 THEN
      LEAVE loop_label;
    END IF;
    SET str = CONCAT(str,x,',');
    SET x = x + 1;
    ITERATE loop_label;
  END LOOP;

  SELECT str;

END//



CREATE OR REPLACE PROCEDURE clear_tables()
BEGIN
    DECLARE cursor_List CURSOR FOR 
      SELECT ItemId, itemname FROM items
    ;
END