package JQ::Utils::Text::Split;
use base Exporter;
use strict;

our @EXPORT = qw( split_text );

=item split_text( $text )

Splits text into substrings
Return array of substrings

=cut

# 

sub split_text
{
    my $text = shift or return;
    my @strings;
    my @chars = split(//, $text);
    my $string_delimiter_re = '[\s]'; 
    my $escape_chars_re = '\\\\';
    my $quote_chars_re = '["]';
    my $quote = ''; # quote char and quoted string flag at one
    my $is_escaped = 0;
    
    my @string = ();
    foreach( @chars ) {
        my $ch = $_;
    
        # escape character
        if( $ch =~ /$escape_chars_re/ && !$is_escaped ) { $is_escaped = 1; next; }
        if( $is_escaped ) { push @string, $ch; $is_escaped = 0; next; }
    
        # ignore string delimiter characters outside of string 
        # or end of string (unquoted)
        if( $ch =~ /$string_delimiter_re/ && !$quote ) {
            next unless @string; # ignore string
            push @strings, join('', @string );
            @string = ();
            next;
        }
        
        # end of quoted string
        if( $ch =~ /$quote_chars_re/ ) {
            if( !$quote ) { $quote = $ch; next; }
            
            if( $ch eq $quote ) {
                $quote = '';
                push @strings, join('', @string );
                @string = ();
                next;
            }
        }
    
        push @string, $ch;
    }
    push @strings, join('', @string ) if @string;
    return @strings;
}

1;
