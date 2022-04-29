USE TPCH;


SELECT /*TPCH_Q1*/
    l_returnflag,
    l_linestatus, 
    sum(l_quantity) as sum_qty, 
    sum(l_extendedprice) as sum_base_price, 
    sum(l_extendedprice * (1 - l_discount)) as sum_disc_price, 
    sum(l_extendedprice * (1 - l_discount) * (1 + l_tax)) as sum_charge, 
    avg(l_quantity) as avg_qty, 
    avg(l_extendedprice) as avg_price, 
    avg(l_discount) as avg_disc, 
    count(*) as count_order
from 
    lineitem
where 
    l_shipdate <= date'1998-12-01' - interval '90' day 
group by
    l_returnflag, 
    l_linestatus
order by 
    l_returnflag, 
    l_linestatus;


SELECT /*TPCH_Q2*/
       s_acctbal, s_name, n_name, 
       p_partkey, p_mfgr, s_address, s_phone, s_comment 
from part, supplier, partsupp, nation, region 
where p_partkey = ps_partkey and 
      s_suppkey = ps_suppkey and 
      p_size = 30 and 
      p_type like '%STEEL' and 
      s_nationkey = n_nationkey and 
      n_regionkey = r_regionkey and 
      r_name = 'ASIA' and 
      ps_supplycost = ( SELECT min(ps_supplycost) 
                        from partsupp, supplier, nation, region 
                        where p_partkey = ps_partkey and 
                              s_suppkey = ps_suppkey and 
                              s_nationkey = n_nationkey and 
                              n_regionkey = r_regionkey and r_name = 'ASIA' ) 
order by s_acctbal desc, n_name, s_name, p_partkey
limit 100;


SELECT
/*
      TPCH_Q3
  */  l_orderkey, 
             o_orderdate, 
             o_shippriority,
             sum(l_extendedprice * (1 - l_discount)) as revenue
      from customer, orders, lineitem 
      where c_mktsegment = 'BUILDING' and 
            c_custkey = o_custkey and 
            l_orderkey = o_orderkey and 
            o_orderdate <  date'1995-03-15' and 
            l_shipdate >  date'1995-03-15' 
      group by l_orderkey, o_orderdate, o_shippriority 
      order by revenue desc, o_orderdate
limit 10;


SELECT  
/*
      TPCH_Q4
  */ o_orderpriority, count(*) as order_count 
from orders 
where o_orderdate >= DATE'1993-07-01' and 
      o_orderdate < DATE'1993-07-01' + interval '3' month and 
      exists ( SELECT * 
               from lineitem 
               where l_orderkey = o_orderkey and 
                     l_commitdate < l_receiptdate ) 
 group by o_orderpriority 
 order by o_orderpriority;
 
 
 SELECT  
 /*
      TPCH_Q5
  */ N_NAME, SUM(L_EXTENDEDPRICE*(1-L_DISCOUNT)) AS REVENUE 
FROM CUSTOMER, ORDERS, LINEITEM, SUPPLIER, NATION, REGION 
WHERE C_CUSTKEY = O_CUSTKEY AND 
      L_ORDERKEY = O_ORDERKEY AND 
      L_SUPPKEY = S_SUPPKEY AND 
      C_NATIONKEY = S_NATIONKEY AND 
      C_NATIONKEY = N_NATIONKEY AND 
      N_REGIONKEY = R_REGIONKEY AND 
      R_NAME = 'EUROPE' AND 
      O_ORDERDATE >= DATE'1995-01-01' AND 
      O_ORDERDATE < DATE'1995-01-01' + interval '1' year
GROUP BY N_NAME 
ORDER BY REVENUE DESC;


SELECT 
/*
      TPCH_Q6
  */ sum(l_extendedprice * l_discount) as revenue 
from lineitem 
where l_shipdate >= DATE'1994-01-01' and 
      l_shipdate < DATE'1994-01-01' + interval  '12' month and 
      l_discount between .06 - 0.01 and .06 + 0.01 and l_quantity < 24;


SELECT 
/*
      TPCH_Q7
  */  supp_nation, cust_nation, l_year, sum(volume) as revenue 
from ( SELECT n1.n_name as supp_nation, 
              n2.n_name as cust_nation,  
              to_number (to_char(l_shipdate, 'yyyy')) as l_year, 
              l_extendedprice * (1 - l_discount) as volume 
      from supplier, lineitem, orders, customer, nation n1, nation n2 
      where s_suppkey = l_suppkey and 
            o_orderkey = l_orderkey and 
            c_custkey = o_custkey and 
            s_nationkey = n1.n_nationkey and 
            c_nationkey = n2.n_nationkey and 
            ( (n1.n_name = 'FRANCE' and n2.n_name = 'GERMANY') or 
              (n1.n_name = 'GERMANY' and n2.n_name = 'FRANCE') ) and 
            l_shipdate between  date'1995-01-01' and date'1996-12-31' ) shipping 
group by supp_nation, cust_nation, l_year order by supp_nation, cust_nation, l_year;


SELECT  
/*
      TPCH_Q8
  */  o_year, sum(case when nation='BRAZIL' then volume else 0 end )/ sum(volume) as mkt_share 
from ( SELECT TO_CHAR(o_orderdate,'YYYY') as o_year, 
              l_extendedprice * (1 - l_discount) as volume, n2.n_name as nation 
       from part, supplier, lineitem, orders, customer, nation n1, nation n2, region 
       where p_partkey = l_partkey and 
             s_suppkey = l_suppkey and 
             l_orderkey = o_orderkey and 
             o_custkey = c_custkey and 
             c_nationkey = n1.n_nationkey and 
             n1.n_regionkey = r_regionkey and 
             r_name = 'AMERICA' and 
             s_nationkey = n2.n_nationkey and 
             o_orderdate between date'1995-01-01' and date'1996-12-31' and 
             p_type = 'ECONOMY ANODIZED STEEL' ) all_nations 
 group by o_year 
 order by o_year;
 
 
 SELECT  
/*
      TPCH_Q9
  */NATION, O_YEAR, SUM(AMOUNT) AS SUM_PROFIT 
FROM (SELECT N_NAME AS NATION, 
             TO_CHAR(O_ORDERDATE, 'YYYY') AS O_YEAR,  
             L_EXTENDEDPRICE*(1-L_DISCOUNT)-PS_SUPPLYCOST*L_QUANTITY AS AMOUNT  
      FROM PART, SUPPLIER, LINEITEM, PARTSUPP, ORDERS, NATION  
      WHERE S_SUPPKEY = L_SUPPKEY AND 
            PS_SUPPKEY= L_SUPPKEY AND 
            PS_PARTKEY = L_PARTKEY AND  
            P_PARTKEY= L_PARTKEY AND 
            O_ORDERKEY = L_ORDERKEY AND 
            S_NATIONKEY = N_NATIONKEY AND 
            P_NAME LIKE '%%green%%') PROFIT 
GROUP BY NATION, O_YEAR ORDER BY SUM_PROFIT, NATION, O_YEAR DESC;


SELECT 
/*
      TPCH_Q10
  */ c_custkey, 
               c_name, 
               sum(l_extendedprice * (1 - l_discount)) as revenue, 
               c_acctbal, 
               n_name, 
               c_address, 
               c_phone, 
               c_comment 
       from customer, orders, lineitem, nation 
       where c_custkey = o_custkey and 
             l_orderkey = o_orderkey and 
             o_orderdate >= DATE'1993-10-01' and 
             o_orderdate < DATE'1993-10-01' + interval '3' month and 
             l_returnflag = 'R' and 
             c_nationkey = n_nationkey 
       group by c_custkey, c_name, c_acctbal, c_phone, n_name, c_address, c_comment 
       order by revenue desc
limit 20;


SELECT 
/*
      TPCH_Q11
  */ ps_partkey, 
       Sum(ps_supplycost * ps_availqty) AS value 
FROM   partsupp, 
       supplier, 
       nation 
WHERE  ps_suppkey = s_suppkey 
       AND s_nationkey = n_nationkey 
       AND n_name = 'MOZAMBIQUE' 
GROUP  BY ps_partkey 
HAVING Sum(ps_supplycost * ps_availqty) > (SELECT 
       Sum(ps_supplycost * ps_availqty) * 0.0001000000 
                                           FROM   partsupp, 
                                                  supplier, 
                                                  nation 
                                           WHERE  ps_suppkey = s_suppkey 
                                                  AND s_nationkey = n_nationkey 
                                                  AND n_name = 'MOZAMBIQUE') 
ORDER  BY value DESC;


SELECT 
/*
      TPCH_Q12
  */ L_SHIPMODE, 
       SUM(CASE WHEN O_ORDERPRIORITY = '1-URGENT' OR O_ORDERPRIORITY = '2-HIGH' THEN 1 ELSE 0 END) AS HIGH_LINE_COUNT, 
       SUM(CASE WHEN O_ORDERPRIORITY <> '1-URGENT' AND O_ORDERPRIORITY <> '2-HIGH' THEN 1 ELSE 0 END ) AS LOW_LINE_COUNT 
FROM ORDERS, LINEITEM 
WHERE O_ORDERKEY = L_ORDERKEY AND 
      L_SHIPMODE IN ('MAIL','SHIP') AND 
      L_COMMITDATE < L_RECEIPTDATE AND 
      L_SHIPDATE < L_COMMITDATE AND 
      L_RECEIPTDATE >= DATE'1994-01-01' AND 
      L_RECEIPTDATE < DATE'1995-09-01' + interval '1' year 
 GROUP BY L_SHIPMODE 
 ORDER BY L_SHIPMODE;

 
SELECT  
 /*
      TPCH_Q13
  */c_count, count(*) as custdist 
from ( SELECT c_custkey, count(o_orderkey)  as  c_count 
       from customer left join orders on c_custkey = o_custkey and o_comment not like '%special%requests%' 
       group by c_custkey ) c_orders 
group by c_count 
order by custdist desc, c_count desc;


SELECT  
/*
      TPCH_Q14
  */100.00 * sum(case when p_type like 'promo%' then l_extendedprice * (1 - l_discount) else 0 end) / sum(l_extendedprice * (1 - l_discount)) as promo_revenue 
from lineitem, part 
where l_partkey = p_partkey and 
      l_shipdate >= date '1995-09-01' and 
      l_shipdate < date '1995-09-01' + interval '1' month;

CREATE VIEW REVENUE0 AS
SELECT l_suppkey                                 AS supplier_no,
SUM(l_extendedprice * ( 1 - l_discount )) AS total_revenue
FROM   lineitem
WHERE  l_shipdate >= DATE '1997-07-01'
AND l_shipdate < DATE '1997-07-01' + interval '3' month
GROUP  BY l_suppkey;


SELECT 
/*
      TPCH_Q15
  */ s_suppkey, 
       s_name, 
       s_address, 
       s_phone, 
       total_revenue 
FROM   supplier, 
       revenue0 
WHERE  s_suppkey = supplier_no 
       AND total_revenue = (SELECT Max(total_revenue) 
                            FROM   revenue0) 
ORDER  BY s_suppkey;



SELECT 
/*
      TPCH_Q16
  */p_brand, p_type, p_size, count(distinct ps_suppkey) as supplier_cnt 
from partsupp, part 
where p_partkey = ps_partkey and 
      p_brand <> 'Brand#45' and 
      p_type not like 'MEDIUM POLISHED%' and 
      p_size in (49, 14, 23, 45, 19, 3, 36, 9) and 
      ps_suppkey not in ( SELECT s_suppkey 
                          from supplier 
                          where s_comment like '%customer%complaints%' ) 
group by p_brand, p_type, p_size 
order by supplier_cnt desc, p_brand, p_type, p_size;


SELECT 
/*
      TPCH_Q17
      BEGIN_OUTLINE_DATA
      PARALLEL(128)
      USE_PX
      END_OUTLINE_DATA
  */ Sum(l_extendedprice) / 7.0 AS avg_yearly 
FROM   lineitem, 
       part 
WHERE  p_partkey = l_partkey 
       AND p_brand = 'Brand#44' 
       AND p_container = 'WRAP PKG' 
       AND l_quantity < (SELECT 0.2 * Avg(l_quantity) 
                         FROM   lineitem 
                         WHERE  l_partkey = p_partkey);


SELECT
/*
      TPCH_Q18
  */ c_name, c_custkey, o_orderkey, o_orderdate, o_totalprice, sum(l_quantity) 
from customer, orders, lineitem 
where o_orderkey in ( SELECT l_orderkey 
                      from lineitem 
                      group by l_orderkey 
                      having sum(l_quantity) > 314 ) and 
      c_custkey = o_custkey and 
      o_orderkey = l_orderkey 
 group by c_name, c_custkey, o_orderkey, o_orderdate, o_totalprice 
 order by o_totalprice desc, o_orderdate 
limit 100;


SELECT 
 /*
      TPCH_Q19
  */Sum(l_extendedprice * ( 1 - l_discount )) AS revenue 
FROM   lineitem, 
       part 
WHERE  p_partkey = l_partkey 
         AND l_shipmode IN ( 'AIR', 'AIR REG' ) 
         AND l_shipinstruct = 'DELIVER IN PERSON'
         AND (( p_partkey = l_partkey 
         AND p_brand = 'Brand#12' 
         AND p_container IN ( 'SM CASE', 'SM BOX', 'SM PACK', 'SM PKG' ) 
         AND l_quantity >= 1 
         AND l_quantity <= 1 + 10 
         AND p_size BETWEEN 1 AND 5 
         AND l_shipmode IN ( 'AIR', 'AIR REG' ) 
         AND l_shipinstruct = 'DELIVER IN PERSON' ) 
        OR ( p_partkey = l_partkey 
             AND p_brand = 'Brand#23' 
             AND p_container IN ( 'MED BAG', 'MED BOX', 'MED PKG', 'MED PACK' ) 
             AND l_quantity >= 10 
             AND l_quantity <= 10 + 10 
             AND p_size BETWEEN 1 AND 10 
             AND l_shipmode IN ( 'AIR', 'AIR REG' ) 
             AND l_shipinstruct = 'DELIVER IN PERSON' ) 
        OR ( p_partkey = l_partkey 
             AND p_brand = 'Brand#34' 
             AND p_container IN ( 'LG CASE', 'LG BOX', 'LG PACK', 'LG PKG' ) 
             AND l_quantity >= 20 
             AND l_quantity <= 20 + 10 
             AND p_size BETWEEN 1 AND 15 
             AND l_shipmode IN ( 'AIR', 'AIR REG' ) 
             AND l_shipinstruct = 'DELIVER IN PERSON' ));


SELECT 
/*
      TPCH_Q20
  */s_name, 
       s_address 
FROM   supplier, 
       nation 
WHERE  s_suppkey IN (SELECT ps_suppkey 
                     FROM   partsupp 
                     WHERE  ps_partkey IN (SELECT p_partkey 
                                           FROM   part 
                                           WHERE  p_name LIKE 'green%') 
                            AND ps_availqty > (SELECT 0.5 * SUM(l_quantity) 
                                               FROM   lineitem 
                                               WHERE  l_partkey = ps_partkey 
                                                      AND l_suppkey = ps_suppkey 
                                                      AND l_shipdate >= DATE 
                                                          '1993-01-01' 
                                                      AND l_shipdate < DATE 
                                                          '1993-01-01' + interval '1' year 
                                              )) 
       AND s_nationkey = n_nationkey 
       AND n_name = 'ALGERIA' 
ORDER  BY s_name;


SELECT 
/*
      TPCH_Q21
  */ s_name, 
               Count(*) numwait 
        FROM   supplier, 
               lineitem l1, 
               orders, 
               nation 
        WHERE  s_suppkey = l1.l_suppkey 
               AND o_orderkey = l1.l_orderkey 
               AND o_orderstatus = 'F' 
               AND l1.l_receiptdate > l1.l_commitdate 
               AND EXISTS (SELECT * 
                           FROM   lineitem l2 
                           WHERE  l2.l_orderkey = l1.l_orderkey 
                                  AND l2.l_suppkey <> l1.l_suppkey) 
               AND NOT EXISTS (SELECT * 
                               FROM   lineitem l3 
                               WHERE  l3.l_orderkey = l1.l_orderkey 
                                      AND l3.l_suppkey <> l1.l_suppkey 
                                      AND l3.l_receiptdate > l3.l_commitdate) 
               AND s_nationkey = n_nationkey 
               AND N_NAME = 'SAUDI ARABIA' 
        GROUP  BY s_name 
        ORDER  BY numwait DESC, 
                  s_name
limit 100;


SELECT  
/*
      TPCH_Q22
  */ cntrycode, 
       Count(*)       AS numcust, 
       Sum(c_acctbal) AS totacctbal 
FROM   (SELECT Substr(c_phone, 1, 2) AS cntrycode, 
               c_acctbal 
        FROM   customer 
        WHERE  Substr(c_phone, 1, 2) IN ( '13', '31', '23', '29', 
                                          '30', '18', '17' ) 
               AND c_acctbal > (SELECT Avg(c_acctbal) 
                                FROM   customer 
                                WHERE  c_acctbal > 0.00 
                                       AND Substr(c_phone, 1, 2) IN ( 
                                           '13', '31', '23', '29', 
                                           '30', '18', '17' )) 
               AND NOT EXISTS (SELECT * 
                               FROM   orders 
                               WHERE  o_custkey = c_custkey)) custsale 
GROUP  BY cntrycode 
ORDER  BY cntrycode;