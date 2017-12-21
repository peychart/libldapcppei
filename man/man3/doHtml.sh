#!/bin/sh

# [ $# -ne 1 ] && >&2 echo "syntax: $0 fileName..." && exit 1

for I in *\.[0-9]\.gz ; do

zcat $I | \

man2html | sed -e '/Return to Main Contents/s;^.*$;<A HREF="http://homepage.gna.org/ldapcppei/miniHowto-1.en.html#ss1.8">Return to Main Contents</A><HR>;' | \

sed -e 's;"\(/cgi-bin/man/man2html?.+\)\(.*\)".*(\(.\))\(.*$\);"\2.\3.html">\2</A></B>(\3)\4;' | \

sed -e 's;"/cgi-bin/man/man2html";"http://savannah.nongnu.org/projects/man2html/";' | \

cat >$(echo $I | sed -e 's/\.gz$//').html

done

