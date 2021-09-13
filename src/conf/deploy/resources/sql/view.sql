create or replace view power_consumption
as
select date(time) as theday, (max(value) - min(value)) as consumption
from Item177
group by theday;

create or replace table Item467
as
select timestamp(theday) as time, consumption as value
from power_consumption;

create or replace table Item467 as select timestamp(theday) as time, consumption as value from power_consumption;

//select CONCAT(YEAR(theday), '-', MONTH(theday)) as power_month, sum(consumption)
//from power_consumption
//group by power_month
//order by power_month asc

// SELECT A.itemid, A.itemname, B.TABLE_NAME, B.TABLE_ROWS from OpenHAB.items as A, `information_schema`.`TABLES` AS B WHERE concat('Item', A.ItemID) = B.TABLE_NAME COLLATE utf8_general_ci AND B.`TABLE_SCHEMA` LIKE 'OpenHAB%' ORDER BY TABLE_ROWS DESC
