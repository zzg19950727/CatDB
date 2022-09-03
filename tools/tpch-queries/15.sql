SELECT s_suppkey,
       s_name,
       s_address,
       s_phone,
       total_revenue
FROM   supplier,
       (SELECT l_suppkey                                 AS supplier_no,
               SUM(l_extendedprice * ( 1 - l_discount )) AS total_revenue
        FROM   lineitem
        WHERE  l_shipdate >= DATE '1997-07-01'
               AND l_shipdate < DATE '1997-07-01' + interval '3' month
        GROUP  BY l_suppkey)REVENUE0
WHERE  s_suppkey = supplier_no
       AND total_revenue = (SELECT Max(total_revenue)
                            FROM   (SELECT l_suppkey
                                           AS
                                           supplier_no,
                                           SUM(l_extendedprice * ( 1
                                               - l_discount )) AS
                                           total_revenue
                                    FROM   lineitem
                                    WHERE  l_shipdate >= DATE '1997-07-01'
                                           AND l_shipdate < DATE '1997-07-01' +
                                                            interval
                                                            '3' month
                                    GROUP  BY l_suppkey)REVENUE0)
ORDER  BY s_suppkey; 