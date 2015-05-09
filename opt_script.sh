#!/bin/bash
# domyslne httpd.conf
for param in 1000 5000 10000 100000
do
  echo "httpd.conf niezmieniane | ab -n $param"
  for i in 1 2 3
  do
    echo `ab -n $param -c 20 http://127.0.0.1/ | egrep "Time taken for tests|Requests per second|Time per request"`
  done
done
# zmieniane httpd.conf
for maxclients in "MaxClients 10" "MaxClients 50" "MaxClients 100" "MaxClients 1000"
do
  service httpd stop >/dev/null 2>&1
  rm -rf /etc/httpd/conf/httpd.conf
  cp /etc/httpd/conf/httpd.conf.kopia /etc/httpd/conf/httpd.conf
  echo "KeepAlive On" >> /etc/httpd/conf/httpd.conf
  echo $maxclients >> /etc/httpd/conf/httpd.conf
  service httpd start >/dev/null 2>&1
  for param in 1000 5000 10000 100000
  do
    echo "KeepAlive On, $maxclients | ab -n $param"
    for i in 1 2 3
    do
      echo `ab -n $param -c 20 http://127.0.0.1/ | egrep "Time taken for tests|Requests per second|Time per request"`
    done
  done
done
