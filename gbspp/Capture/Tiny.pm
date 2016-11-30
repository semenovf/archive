sub BEGIN{require 5.006;}package gbspp::Capture::Tiny;
use warnings;
use strict;
our$VERSION='0.44';
use Carp ();
use Exporter ();
use IO::Handle ();
use File::Spec ();
use File::Temp ('tempfile','tmpnam');
use Scalar::Util ('reftype','blessed');
sub BEGIN{local$@;
*PerlIO::get_layers=sub{return();}unless eval{do{require PerlIO;
'PerlIO'->can('get_layers')}};}my(%api)=('capture',[1,1,0,0],'capture_stdout',[1,0,0,0],'capture_stderr',[0,1,0,0],'capture_merged',[1,1,1,0],'tee',[1,1,0,1],'tee_stdout',[1,0,0,1],'tee_stderr',[0,1,0,1],'tee_merged',[1,1,1,1]);
foreach my $sub(keys%api){my$args=join(', ',@{$api{$sub};});
eval"sub $sub(&;\@) {unshift \@_, $args; goto \\&_capture_tee;}";}our(@ISA)='Exporter';
our(@EXPORT_OK)=keys%api;
our(%EXPORT_TAGS)=('all',\@EXPORT_OK);
my$IS_WIN32=$^O eq 'MSWin32';
our$TIMEOUT=30;
my(@cmd)=($^X,'-C0','-e',"use Fcntl;\n\$SIG{HUP}=sub{exit};\nif ( my \$fn=shift ) {\n    sysopen(my \$fh, qq{\$fn}, O_WRONLY|O_CREAT|O_EXCL) or die \$!;\n    print {\$fh} \$\$;\n    close \$fh;\n}\nmy \$buf; while (sysread(STDIN, \$buf, 2048)) {\n    syswrite(STDOUT, \$buf); syswrite(STDERR, \$buf);\n}\n");
sub _relayer{my($fh,$layers)=@_;
binmode$fh,':raw';
while(1<(my$layers=()=&PerlIO::get_layers($fh,'output',1))){do{binmode$fh,':pop'};}my(@to_apply)=@$layers;
shift@to_apply;
binmode$fh,':'.join(':',@to_apply);}sub _name{my$glob=shift();
no strict 'refs';
return*{$glob;}{'NAME'};}sub _open{Carp::confess('Error from open('.join(', ',@_)."): $!")unless open$_[0],$_[1];}sub _close{Carp::confess('Error from close('.join(', ',@_)."): $!")unless close$_[0];}my(%dup,%proxy_count);
sub _proxy_std{my%proxies;
unless(defined fileno STDIN){++$proxy_count{'stdin'};
if(defined$dup{'stdin'}){_open\*STDIN,'<&='.fileno($dup{'stdin'});}else{_open\*STDIN,'<'.'File::Spec'->devnull;
_open$dup{'stdin'}='IO::Handle'->new,'<&=STDIN';}$proxies{'stdin'}=\*STDIN;
binmode STDIN,':utf8' if$]>=5.008;}unless(defined fileno STDOUT){++$proxy_count{'stdout'};
if(defined$dup{'stdout'}){_open\*STDOUT,'>&='.fileno($dup{'stdout'});}else{_open\*STDOUT,'>'.'File::Spec'->devnull;
_open$dup{'stdout'}='IO::Handle'->new,'>&=STDOUT';}$proxies{'stdout'}=\*STDOUT;
binmode STDOUT,':utf8' if$]>=5.008;}unless(defined fileno STDERR){++$proxy_count{'stderr'};
if(defined$dup{'stderr'}){_open\*STDERR,'>&='.fileno($dup{'stderr'});}else{_open\*STDERR,'>'.'File::Spec'->devnull;
_open$dup{'stderr'}='IO::Handle'->new,'>&=STDERR';}$proxies{'stderr'}=\*STDERR;
binmode STDERR,':utf8' if$]>=5.008;}return%proxies;}sub _unproxy{my(%proxies)=@_;
foreach my $p(keys%proxies){--$proxy_count{$p};
unless($proxy_count{$p}){_close$proxies{$p};
_close$dup{$p}unless$]<5.008;
delete$dup{$p};}}}sub _copy_std{my%handles;
foreach my $h('stdout','stderr','stdin'){next if$h eq 'stdin' and not$IS_WIN32;
my$redir=$h eq 'stdin'?'<&':'>&';
_open$handles{$h}='IO::Handle'->new,$redir.uc($h);}return\%handles;}sub _open_std{my($handles)=@_;
_open\*STDIN,'<&'.fileno($handles->{'stdin'})if defined$handles->{'stdin'};
_open\*STDOUT,'>&'.fileno($handles->{'stdout'})if defined$handles->{'stdout'};
_open\*STDERR,'>&'.fileno($handles->{'stderr'})if defined$handles->{'stderr'};}sub _start_tee{my($which,$stash)=@_;
$stash->{$_}{$which}='IO::Handle'->new foreach('tee','reader');
pipe$stash->{'reader'}{$which},$stash->{'tee'}{$which};
select+(select($stash->{'tee'}{$which}),$|=1)[0];
$stash->{'new'}{$which}=$stash->{'tee'}{$which};
$stash->{'child'}{$which}={'stdin',$stash->{'reader'}{$which},'stdout',$stash->{'old'}{$which},'stderr',$stash->{'capture'}{$which}};
$stash->{'flag_files'}{$which}=scalar tmpnam();
if($IS_WIN32){my$old_eval_err=$@;
undef$@;
eval 'use Win32API::File qw/GetOsFHandle SetHandleInformation fileLastError HANDLE_FLAG_INHERIT INVALID_HANDLE_VALUE/ ';
my$os_fhandle=GetOsFHandle($stash->{'tee'}{$which});
my$result=SetHandleInformation($os_fhandle,HANDLE_FLAG_INHERIT(),0);
_open_std$stash->{'child'}{$which};
$stash->{'pid'}{$which}=system(1,@cmd,$stash->{'flag_files'}{$which});
$@=$old_eval_err;}else{_fork_exec($which,$stash);}}sub _fork_exec{my($which,$stash)=@_;
my$pid=fork;
if(not defined$pid){Carp::confess("Couldn't fork(): $!");}elsif($pid==0){untie*STDIN;
untie*STDOUT;
untie*STDERR;
_close$stash->{'tee'}{$which};
_open_std$stash->{'child'}{$which};
exec@cmd,$stash->{'flag_files'}{$which};}$stash->{'pid'}{$which}=$pid;}my$have_usleep=eval q[use Time::HiRes 'usleep'; 1];
sub _files_exist{return 1 if@_==grep({-f$_;}@_);
Time::HiRes::usleep(1000)if$have_usleep;
return 0;}sub _wait_for_tees{my($stash)=@_;
my$start=time;
my(@files)=values%{$$stash{'flag_files'};};
my$timeout=defined$ENV{'PERL_CAPTURE_TINY_TIMEOUT'}?$ENV{'PERL_CAPTURE_TINY_TIMEOUT'}:$TIMEOUT;
'???' until _files_exist@files or$timeout and time-$start>$timeout;
Carp::confess('Timed out waiting for subprocesses to start')unless _files_exist@files;
unlink$_ foreach(@files);}sub _kill_tees{my($stash)=@_;
if($IS_WIN32){close$_ foreach(values%{$$stash{'tee'};});
my$start=time;
1 until wait==-1 or time-$start>30;}else{_close$_ foreach(values%{$$stash{'tee'};});
waitpid$_,0 foreach(values%{$$stash{'pid'};});}}sub _slurp{my($name,$stash)=@_;
my($fh,$pos)=map({$stash->{$_}{$name};}'capture','pos');
die"Couldn't seek on capture handle for $name\n" unless seek$fh,$pos,0;
my$text=do{local$/;
scalar<$fh>};
return defined$text?$text:'';}sub _capture_tee{my($do_stdout,$do_stderr,$do_merge,$do_tee,$code,@opts)=@_;
my(%do)=($do_stdout?('stdout',1):(),$do_stderr?('stderr',1):());
Carp::confess("Custom capture options must be given as key/value pairs\n")unless@opts%2==0;
my$stash={'capture',{@opts}};
foreach$_(keys%{$$stash{'capture'};}){my$fh=$stash->{'capture'}{$_};
Carp::confess("Custom handle for $_ must be seekable\n")unless ref$fh eq 'GLOB' or&blessed($fh)and$fh->isa('IO::Seekable');}local*CT_ORIG_STDIN=*STDIN;
local*CT_ORIG_STDOUT=*STDOUT;
local*CT_ORIG_STDERR=*STDERR;
my(%layers)=('stdin',[&PerlIO::get_layers(\*STDIN)],'stdout',[&PerlIO::get_layers(\*STDOUT,'output',1)],'stderr',[&PerlIO::get_layers(\*STDERR,'output',1)]);
$layers{'stdout'}=[&PerlIO::get_layers(tied*STDOUT)]if tied*STDOUT and&reftype(tied*STDOUT)eq 'GLOB';
$layers{'stderr'}=[&PerlIO::get_layers(tied*STDERR)]if tied*STDERR and&reftype(tied*STDERR)eq 'GLOB';
my%localize;
++$localize{'stdin'},local*STDIN if grep{$_ eq 'scalar';}@{$layers{'stdin'};};
++$localize{'stdout'},local*STDOUT if$do_stdout and grep{$_ eq 'scalar';}@{$layers{'stdout'};};
++$localize{'stderr'},local*STDERR if$do_stderr||$do_merge and grep{$_ eq 'scalar';}@{$layers{'stderr'};};
++$localize{'stdin'},local*STDIN,_open(\*STDIN,'<&=0')if tied*STDIN and$]>=5.008;
++$localize{'stdout'},local*STDOUT,_open(\*STDOUT,'>&=1')if$do_stdout and tied*STDOUT and$]>=5.008;
++$localize{'stderr'},local*STDERR,_open(\*STDERR,'>&=2')if$do_stderr||$do_merge and tied*STDERR and$]>=5.008;
my(%proxy_std)=_proxy_std();
$layers{'stdout'}=[&PerlIO::get_layers(\*STDOUT,'output',1)]if$proxy_std{'stdout'};
$layers{'stderr'}=[&PerlIO::get_layers(\*STDERR,'output',1)]if$proxy_std{'stderr'};
$stash->{'old'}=_copy_std();
$stash->{'new'}={%{$stash->{'old'};}};
foreach$_(keys%do){$stash->{'new'}{$_}=($stash->{'capture'}{$_}||='File::Temp'->new);
die"Could not seek on capture handle for $_\n" unless seek$stash->{'capture'}{$_},0,2;
$stash->{'pos'}{$_}=tell$stash->{'capture'}{$_};
_start_tee$_,$stash if$do_tee;}_wait_for_tees$stash if$do_tee;
$stash->{'new'}{'stderr'}=$stash->{'new'}{'stdout'}if$do_merge;
_open_std$stash->{'new'};
my($exit_code,$inner_error,$outer_error,$orig_pid,@result);
{$orig_pid=$$;
local*STDIN=*CT_ORIG_STDIN if$localize{'stdin'};
_relayer\*STDOUT,$layers{'stdout'}if$do_stdout;
_relayer\*STDERR,$layers{'stderr'}if$do_stderr;
my$old_eval_err=$@;
undef$@;
eval{do{@result=&$code();
$inner_error=$@}};
$exit_code=$?;
$outer_error=$@;
'STDOUT'->flush if$do_stdout;
'STDERR'->flush if$do_stderr;
$@=$old_eval_err;}_open_std$stash->{'old'};
_close$_ foreach(values%{$$stash{'old'};});
_relayer\*STDOUT,$layers{'stdout'}if$do_stdout;
_relayer\*STDERR,$layers{'stderr'}if$do_stderr;
_unproxy%proxy_std;
_kill_tees$stash if$do_tee;
my%got;
if($orig_pid==$$ and defined wantarray||$do_tee&&keys%localize){foreach$_(keys%do){_relayer$stash->{'capture'}{$_},$layers{$_};
$got{$_}=_slurp($_,$stash);}print CT_ORIG_STDOUT$got{'stdout'}if$do_stdout and$do_tee and$localize{'stdout'};
print CT_ORIG_STDERR$got{'stderr'}if$do_stderr and$do_tee and$localize{'stderr'};}$?=$exit_code;
$@=$inner_error if$inner_error;
die$outer_error if$outer_error;
return unless defined wantarray;
my@return;
push@return,$got{'stdout'}if$do_stdout;
push@return,$got{'stderr'}if$do_stderr and not$do_merge;
push@return,@result;
return wantarray?@return:$return[0];}'???';
