#!/usr/bin/perl -w

use locale;
use POSIX qw (locale_h);

setlocale(LC_CTYPE, 'ru_RU.CP1251'); # ЕМНИП, в windows локаль называется 'Russian_Russia.1251'
#

@goroda = ("Москва","London","Ярославль","Berlin");

$text = "london москва berlin ярославль Москва Berlin";



foreach $gorod(@goroda)
{

   chomp($gorod);


   if ($text =~ /$gorod/i)
      {
      $text =~ s/\b$gorod\b/<i>$gorod<\/i>/ig;
      }

}

print "$text";
