package JQ::WebApp;
use Encode qw(decode encode);
#use Path::Class qw(file);  # Export a short constructor
use CGI::Simple; # for cgi support in WebApp class development stage
#use CGI;# qw(:standard);
use JQ::Global;
use JQ::Webject;
use base qw(JQ::App);
use strict;
use warnings;

package JQ::Webject::Body;
use base 'JQ::Webject';
sub render
{
    my $self = shift or die;
    my $attrs = $self->SUPER::attrs;
    my $html = '<body';
    $html .= ' ' . $attrs if $attrs;
    $html .= '>';
    $html .= $self->SUPER::render;
    $html .= '</body>';

    return $html;
}

package JQ::WebApp;

sub XHTML_1_0_Strict       { '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">' }
sub XHTML_1_0_Transitional { '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">' }
sub XHTML_1_0_Frameset     { '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">' }
sub XHTML_1_0_Mobile       { '<!DOCTYPE html PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.0//EN" "http://www.wapforum.org/DTD/xhtml-mobile10.dtd">' }
sub XHTML_1_1              { '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">' }
sub DOCTYPE_DEFAULT        { XHTML_1_0_Transitional }

sub UA_TYPE_UNKNOWN   { 0 }
sub UA_TYPE_FIREFOX   { 1 }
sub UA_TYPE_IE        { 2 }
sub UA_TYPE_CHROME    { 3 }
sub UA_TYPE_SAFARI    { 4 }
sub UA_TYPE_OPERA     { 5 }
sub UA_TYPE_KONQUEROR { 6 }
sub UA_TYPE_STRING    { ['unknown', 'Farefox', 'IE', 'Chrome', 'Safari', 'Opera', 'Konqueror'] }

sub UA_OS_TYPE_UNKNOWN { 0 }
sub UA_OS_TYPE_WIN     { 1 }
sub UA_OS_TYPE_LINUX   { 2 }
sub UA_OS_TYPE_MAC     { 3 }
sub UA_OS_TYPE_STRING  { ['unknown', 'Win', 'Linux', 'Mac'] }

sub TARGET_WIN_THIS    { '_self' }
sub TARGET_WIN_NEW     { '_blank' }

=head2 $webApp->baseAddr() - get/set base address
    see <base href="base_addr" ... />
=head2 $webApp->targetWindow() - get/set target window
    see <base target="target" ... />
=cut

__PACKAGE__->mk_accessors(qw(doctype title targetWindow baseAddr charset));

sub new
{
    my ($class, %args) = @_;
    my $self = bless $class->SUPER::new(%args), $class;
    $self->header(-type=>'text/html',-charset=>'utf-8');
    $self->doctype(DOCTYPE_DEFAULT);
    $self->title('<-- Insert title here -->');
    $self->charset('utf-8');
    $self->baseAddr('');
    $self->targetWindow( TARGET_WIN_THIS );
    $self->userAgentId( $ENV{'HTTP_USER_AGENT'} );
    return $self;
}

sub userAgentType
{
    my $self = shift or die;
    return $self->{'-userAgentType'};
}

sub userAgentTypeString
{
    my $self = shift or die;
    return UA_TYPE_STRING->[$self->{'-userAgentType'}];
}

sub userAgentVersion
{
    my $self = shift or die;
    return $self->{'-userAgentVersion'};
}

sub userAgentOsType
{
    my $self = shift or die;
    return $self->{'-userAgentOsType'};
}

sub userAgentOsTypeString
{
    my $self = shift or die;
    return UA_OS_TYPE_STRING->[$self->{'-userAgentOsType'}];
}

sub userAgentOsVersion
{
    my $self = shift or die;
    return $self->{'-userAgentOsVersion'};
}


=head2 see http://user-agent-string.info/parse
=head2
    -userAgentType         - С‚РёРї Р°РіРµРЅС‚Р° (РЅР°Р·РІР°РЅРёРµ) (СЃРј. UA_TYPE_XXXX )
    -userAgentVersion          - РІРµСЂСЃРёСЏ Р°РіРµРЅС‚Р° (С‡РёСЃР»Рѕ СЃ РїР»Р°РІР°СЋС‰РµР№ С‚РѕС‡РєРѕР№)
    -userAgentOsType      - СЃРµРјРµР№СЃС‚РІРѕ РћРЎ (СЃРј. UA_OS_TYPE_XXXX )
    -userAgentOsVersion       - РІРµСЂСЃРёСЏ РћРЎ (С‡РёСЃР»Рѕ СЃ РїР»Р°РІР°СЋС‰РµР№ С‚РѕС‡РєРѕР№ )

=head2 Examples
    Firefox: 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2.10) Gecko/20100914 Firefox/3.6.10'
    IE8    : 'Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)'
    Chrome : 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/534.3 (KHTML, like Gecko) Chrome/6.0.472.62 Safari/534.3'
    Safari : 'Mozilla/5.0 (Windows; U; Windows NT 5.1; ru-RU) AppleWebKit/531.22.7 (KHTML, like Gecko) Version/4.0.5 Safari/531.22.7'
    Opera  : 'Opera/9.63 (Windows NT 5.1; U; en) Presto/2.1.1'
=cut
sub userAgentId
{
    my $self = shift or die;
    my $uaId = shift;

    return $self->{'-userAgentId'} unless defined $uaId;

    $self->{'-userAgentId'}        = $uaId;
    $self->{'-userAgentType'}      = UA_TYPE_UNKNOWN;
    $self->{'-userAgentVersion'}   = 0.0;
    $self->{'-userAgentOsType'}    = UA_OS_TYPE_UNKNOWN;
    $self->{'-userAgentOsVersion'} = 0.0;

    while( 1 ) {
        $uaId =~ /MSIE[ \+\/]*([0-9][\.0-9]*)/ && do {
            $self->{'-userAgentType'} = UA_TYPE_IE;
            $self->{'-userAgentVersion'} = $1;
            last;
        };

        $uaId =~ /Firefox[ \+\/]*([0-9][\.0-9]*)/ && do {
            $self->{'-userAgentType'} = UA_TYPE_FIREFOX;
            $self->{'-userAgentVersion'} = $1;
            last;
        };

        $uaId =~ /Chrome[ \+\/]*([0-9][\.0-9]*)/ && do {
            $self->{'-userAgentType'} = UA_TYPE_CHROME;
            $self->{'-userAgentVersion'} = $1;
            last;
        };

        # РџСЂРѕРІРµСЂРєСѓ Safari РІС‹РїРѕР»РЅСЏС‚СЊ РїРѕСЃР»Рµ Chrome, С‚.Рє. РёС… РёРґРµРЅС‚РёС„РёРєР°С‚РѕСЂС‹ РїРѕС…РѕР¶Рё -
        # 'Safari' РїСЂРёСЃСѓС‚СЃС‚РІСѓРµС‚ Рё С‚Р°Рј Рё С‚Р°Рј
        $uaId =~ /Safari[ \+\/]*([0-9][\.0-9]*)/ && do {
            $self->{'-userAgentType'} = UA_TYPE_SAFARI;
            $self->{'-userAgentVersion'} = $1;
            last;
        };

        $uaId =~ /Opera[ \+\/]*([0-9][\.0-9]*)/ && do {
            $self->{'-userAgentType'} = UA_TYPE_OPERA;
            $self->{'-userAgentVersion'} = $1;
            last;
        };

        $uaId =~ /Konqueror[ \+\/]*([0-9][\.0-9]*)/ && do {
            $self->{'-userAgentType'} = UA_TYPE_KONQUEROR;
            $self->{'-userAgentVersion'} = $1;
            last;
        };

        last;
    }

    while( 1 ) {
        $uaId =~ /Windows/ && do {
            $self->{'-userAgentOsType'} = UA_OS_TYPE_WIN;
#            $self->{'-userAgentOsVersion'} = $1;
            last;
        };
        $uaId =~ /Linux/ && do {
            $self->{'-userAgentOsType'} = UA_OS_TYPE_LINUX;
#            $self->{'-userAgentOsVersion'} = $1;
            last;
        };
        last;
    }

=head1


    my ($self, $http_user_agent) = @_;
    my ($uatype, $uaver, $ostype, $osver, $arch, $lang);
    my ($ua);



    # ostype/osver
    $ostype = "unknown"; # Windows, Macintosh, Linux, FreeBSD, HP-UX, SunOS
    $osver  = "unknown"; # 16, 3.1, 95, 98, 2000, ME, CE, NT 5.1
    $arch   = "unknown"; # i386, i586, i686, PPC
    $lang   = "";        # en, en-US, ja, de

    $ua = $http_user_agent;
    $ua =~ s/\+/ /g;
    $ua =~ s/Service Pack /SP/g;
    if ($ua =~ /Win/) {
        if ($ua =~ /Win16/) {
            $ostype = "Windows";
            $osver = "16";
        }
        elsif ($ua =~ /Win32/) {
            $ostype = "Windows";
            $osver = "32";
        }
        elsif ($ua =~ /Win(9[58x])/) {
            $ostype = "Windows";
            $osver = $1;
        }
        elsif ($ua =~ /Win(NT *[SP0-9. ]*)/) {
            $ostype = "Windows";
            $osver = $1;
            $osver =~ s/ +$//;
        }
        elsif ($ua =~ /Windows *([239MCX][A-Z0-9. \/]*)/) {
            $ostype = "Windows";
            $osver = $1;
            $osver =~ s/ +$//;
        }
    }
    if ($ostype eq "unknown") {   # haven't found it yet
        if ($ua =~ /Linux/) {
            $ostype = "Linux";
            if ($ua =~ /Linux +([0-9][0-9\.a-z-]*) +([a-zA-Z0-9-]+)/) {
                $osver = $1;
                $arch = $2;
            }
            elsif ($ua =~ /Linux +([0-9][0-9\.a-z-]*)/) {
                $osver = $1;
            }
        }
        elsif ($ua =~ /X11/) {
            $ostype = "X11";
        }
    }

=cut
}

sub mainWebject
{
    my $self = shift or die;
    $self->{'-main-webject'} = JQ::Webject::Body->new unless defined $self->{'-main-webject'};
    return $self->{'-main-webject'};
}

sub scriptDir
{
    my $self = shift or die;
    unless( $self->{-scriptDir} ) {
        if( $ENV{'SCRIPT_FILENAME'} ) {
            my $scrFile = Path::Class::File->new( $ENV{'SCRIPT_FILENAME'} );
            $self->{-scriptDir} = $scrFile->dir;
        } else {
            $self->{-scriptDir} = $ENV{'DOCUMENT_ROOT'};
        }
    }
    return $self->{-scriptDir};
}

sub _cgi
{
    my $self = shift or die;
    unless( defined $self->{-_cgi} ) {
        $self->{-_cgi} = CGI::Simple->new;
    }
    return $self->{-_cgi}
}

sub url_encode
{
    my $self = shift or die;
    return $self->_cgi()->url_encode(@_);
}

sub url_decode
{
    my $self = shift or die;
    return $self->_cgi()->url_decode(@_);
}


sub param
{
    my $self = shift or die;
    return $self->_cgi()->param(@_);
}


sub paramDefault
{
    my $self = shift or die;
    my $pname = shift;
    my $default = shift;
    return undef unless defined $pname;
    my $val = $self->_cgi()->param($pname);
    return defined $val ? $val : $default;
}


sub header
{
    my $self = shift or die;
    return $self->{-header} unless scalar(@_) > 0;
    $self->{-header} = $self->_cgi()->header(@_);
    return $self;
}


sub includeStylesheet
{
    my $self = shift or die;
    push @{$self->{'-includeStylesheet'}}, @_;
}

sub addStyle
{
    my $self = shift or die;
    push @{$self->{'-styles'}}, @_;
}

sub addScript
{
    my $self = shift or die;
    push @{$self->{'-scripts'}}, @_;
}


sub handleException
{
    my $self = shift or die;
    my $ex = shift;
    my $errmsg = ref $ex ? $$ex : $ex; # TODO correction need

    my $html = '';
    $html .= $self->header;
    $html .= $self->doctype;
    $html .= '<html><head>';
    $html .= '<title>'.$self->title.'</title>';
    $html .= '<base href="' . $self->baseAddr . '" target="' . $self->targetWindow . '" />';
    $html .= '<meta http-equiv="content-type" content="text/html; charset=' . $self->charset . '" />';

    if( defined $self->{'-includeStylesheet'} ) {
        foreach( @{$self->{'-includeStylesheet'}} ) {
            $html .= '<link rel="stylesheet" type="text/css" href="' . $_ . '" />';
        }
    }

    $html .= '</head>';
    $html .= '<h3 style="color:#f00;">Error(s):</h3>';

    chomp $errmsg;
    $errmsg =~ s/\n/<\/li><li>/mg;
    $errmsg = '<li>'.$errmsg;

    $html .= '<ul>'.$errmsg.'</ul>';
    $html .= '</html>';
    print $html;
}


sub exec
{
    my $self = shift or die;
    $self->ctor();
    my $html = '';
    $html .= $self->header;
    $html .= $self->doctype;
    $html .= '<html><head>';
    $html .= '<title>'.$self->title.'</title>';
    $html .= '<base href="' . $self->baseAddr . '" target="' . $self->targetWindow . '" />';
    $html .= '<meta http-equiv="content-type" content="text/html; charset=' . $self->charset . '" />';

    if( defined $self->{'-includeStylesheet'} ) {
        foreach( @{$self->{'-includeStylesheet'}} ) {
            $html .= '<link rel="stylesheet" type="text/css" href="' . $_ . '" />';
        }
    }

    if( defined $self->{'-styles'} ) {
        $html .= '<style type="text/css">';
        foreach( @{$self->{'-styles'}} ) {
            $html .= "$_\n";
        }
        $html .= '</style>';
    }

    if( defined $self->{'-scripts'} ) {
        $html .= '<script type="text/javascript">';
        foreach( @{$self->{'-scripts'}} ) {
            $html .= "$_\n";
        }
        $html .= '</script>';
    }

    $html .= '</head>';


    print encode( 'utf8', $html );
    print encode( 'utf8', $self->mainWebject()->render );
    print '</html>';
}

#
# must be overrided in child classes
sub ctor
{
    my $self = shift or die;
    my $mainWebject = $self->mainWebject;
    $mainWebject->add( '<h1>Hello, World!</h1>' );
}

1;
