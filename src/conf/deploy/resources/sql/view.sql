create or replace view power_consumption
as
select date(time) as theday, (max(value) - min(value)) as consumption
from Item177
group by theday;

create or replace view Item180
as
select timestamp(theday) as time, consumption as value
from power_consumption;


//select CONCAT(YEAR(theday), '-', MONTH(theday)) as power_month, sum(consumption)
//from power_consumption
//group by power_month
//order by power_month asc

