#!/bin/bash
mkdir -p disk-model/{A..E} && \
  mkdir -p disk-model/.admin && \
  touch $_/{journal.txt,monitoring.log} && \
  echo 'exp(x * a)' > disk-model/.admin/journal.txt.raw && \
  date +"%a_%b_%d_%H:%M:%S_%Y" > disk-model/.admin/monitoring.log.raw && \
  openssl genrsa -out disk-model/.admin/pr.pem 8192 && \
  openssl rsa -in disk-model/.admin/pr.pem -out disk-model/.admin/pub.pem -outform PEM -pubout && \
  openssl rsautl -encrypt -inkey ./disk-model/.admin/pub.pem -pubin -in disk-model/.admin/journal.txt.raw -out disk-model/.admin/journal.txt && \
  rm -rf disk-model/.admin/journal.txt.raw && \
  openssl rsautl -encrypt -inkey ./disk-model/.admin/pub.pem -pubin -in disk-model/.admin/monitoring.log.raw -out disk-model/.admin/monitoring.log && \
  rm -rf disk-model/.admin/monitoring.log.raw
