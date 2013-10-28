#!/usr/bin/perl -w

my $subst_hash = {
    'query'=>'query substr',
    'qw'=>'QW'
};

my $url_tmpl = 'http://www.google.ru/search?q=[%query%]&[%qw%]ie=[%qw%]utf-8&oe=utf-8';
while( my ($key, $substr) = each( %{$subst_hash} ) ) {
    $url_tmpl =~ s/\[%$key%\]/$substr/g;
}

print $url_tmpl, "\n";


0;
