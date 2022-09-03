SELECT Sum(l_extendedprice) / 7.0 AS avg_yearly
FROM   lineitem,
       part
WHERE  p_partkey = l_partkey
       AND p_brand = 'Brand#44'
       AND p_container = 'WRAP PKG'
       AND l_quantity < (SELECT 0.2 * Avg(l_quantity)
                         FROM   lineitem
                         WHERE  l_partkey = p_partkey); 