explain
SELECT cntrycode,
       Count(*)       AS numcust,
       Sum(c_acctbal) AS totacctbal
FROM   (SELECT Substr(c_phone, 1, 2) AS cntrycode,
               c_acctbal
        FROM   customer
        WHERE  Substr(c_phone, 1, 2) IN ( '20', '40', '22', '30',
                                                    '39', '42', '21' )
               AND c_acctbal > (SELECT Avg(c_acctbal)
                                FROM   customer
                                WHERE  c_acctbal > 0.00
                                       AND Substr(c_phone, 1, 2) IN (
                                           '20', '40', '22', '30',
                                           '39', '42', '21' ))
               AND NOT EXISTS (SELECT *
                               FROM   orders
                               WHERE  o_custkey = c_custkey)) AS custsale
GROUP  BY cntrycode
ORDER  BY cntrycode; 