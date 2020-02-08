#!/bin/bash

echo "$1";

request=curl "https://openapi.naver.com/v1/language/translate" \
-H "Content-Type: application/x-www-form-urlencoded; charset=UTF-8" \
-H "X-Naver-Client-Id: 6HX5RhhlQsQtk9lSvMGE" \
-H "X-Naver-Client-Secret: Guntfo2wA2" -v \
-d "source=ko&target=en&text=".$1 -v

