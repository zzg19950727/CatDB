explain
SELECT c_name,
       c_custkey,
       o_orderkey,
       o_orderdate,
       o_totalprice,
       Sum(l_quantity)
FROM   customer,
       orders,
       lineitem
WHERE  o_orderkey IN (SELECT l_orderkey
                      FROM   lineitem
                      GROUP  BY l_orderkey
                      HAVING Sum(l_quantity) > 314)
       AND c_custkey = o_custkey
       AND o_orderkey = l_orderkey
GROUP  BY c_name,
          c_custkey,
          o_orderkey,
          o_orderdate,
          o_totalprice
ORDER  BY o_totalprice DESC,
          o_orderdate
LIMIT  100; 