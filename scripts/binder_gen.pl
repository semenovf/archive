#!/bin/perl -w

sub binder_def
{
	defined $_[0] or die "Expected number of arguments";
	my $nargs = $_[0];
	my $isMethodBinder = (defined $_[1] ? $_[1] : 0);
	
	my $binderName = $isMethodBinder
	   ? "binder_method${nargs}"
	   : "binder_function${nargs}";

#    my $binderName = $isMethodBinder
#       ? "binder_method"
#       : "binder";
       
	my $r = [];
	
	my $s1 = $isMethodBinder 
	   ? 'template <typename Class, typename Return' 
	   : 'template <typename Return';
	for (my $i = 1; $i <= $nargs; ++$i) {
		$s1 .= ", typename Arg${i}";
	}
	$s1 .= '>';

	push $r, $s1;
	
    push $r, $isMethodBinder 
		  ? "class ${binderName} : public binder_method_base<Class, Return>"
          : "class ${binderName} : public binder_function_base<Return>";
    
    push $r, '{';
    
    for (my $i = 1; $i <= $nargs; ++$i) {
        push $r, "    typename binder_type_trait<Arg${i}>::type _a${i};";
    }
    
    push $r, '';
    push $r, 'public:';
    push $r, $isMethodBinder 
        ? '    typedef binder_method_base<Class, Return> base_class;'
        : '    typedef binder_function_base<Return> base_class;';
    push $r, '    typedef Return return_type;';
    
    my $s2 = $isMethodBinder
        ? '    typedef return_type (Class::* funcptr_type) ('
        : '    typedef return_type (* funcptr_type) (';
    if ($nargs > 0) {
    	$s2 .= 'Arg1';
    }
    for (my $i = 2; $i <= $nargs; ++$i) {
    	$s2 .= ", Arg${i}";
    }
    $s2 .= ');';
    
    push $r, $s2;
    push $r, '';

    my $s3 = $isMethodBinder 
        ? "    $binderName (Class * p, funcptr_type f"
        : "    $binderName (funcptr_type f";
    
    for (my $i = 1; $i <= $nargs; ++$i) {
    	$s3 .= ", Arg${i} a${i}";
    }
    $s3 .= ')';
    
    push $r, $s3;  
    push $r, "        : base_class(sizeof($binderName)";
    
    push $r, $isMethodBinder 
        ? '                , p, reinterpret_cast<typename base_class::funcptr_type>(f))'
        : '                , reinterpret_cast<typename base_class::funcptr_type>(f))';
    
    for (my $i = 1; $i <= $nargs; ++$i) {
        push $r, "        , _a${i}(a${i})";
    }
    push $r, '    {}';
    
    push $r, '';
    push $r, "    virtual ~${binderName} () {}";
    push $r, '';
    push $r, '    virtual return_type operator () () const';
    push $r, '    {';

    push $r, '        funcptr_type f = reinterpret_cast<funcptr_type>(this->_f);';
    my $s4 = '';
    if ($isMethodBinder) {
        $s4 .= '        return (base_class::_p->*f)(';
    } else {
        $s4 .= '        return f(';
    }
    if ($nargs > 0) {
        $s4 .= '_a1';
    }
    
    for (my $i = 2; $i <= $nargs; ++$i) {
        $s4 .= ", _a${i}";
    }
    $s4 .= ');';
    
    push $r, $s4;
    
    push $r, '    }';
    push $r, '};';
    push $r, '';
    
    return $r;
}


my ($sec
    , $min
    , $hour
    , $mday
    , $mon
    , $year
    , $wday
    , $yday
    , $isdst) = localtime(time);
my $now = sprintf("%04d-%02d-%02d %02d:%02d:%02d"
    , $year + 1900
    , $mon + 1
    , $mday, $hour, $min, $sec);

my $thisScriptName = $0;
my $numberOfParms = 8;

if (defined $ARGV[0]) {
    $numberOfParms = 0 + $ARGV[0];
}

my $header = <<"END_OF_CODE";
/* AUTOMATICALLY GENERATED BY '${thisScriptName}' */
/*
 * binder.hpp
 *
 *  Created on: ${now}
 *      Author: wladt
 */

#ifndef __PFS_BINDER_HPP__
#define __PFS_BINDER_HPP__

namespace pfs {

template <typename T>
struct binder_type_trait { typedef T type; };

template <typename T>
struct binder_type_trait<const T &> { typedef T type; };

template <typename Return>
class binder_base
{
public:
    typedef Return return_type;

protected:
    size_t _size;

    binder_base (size_t size)
        : _size(size)
    {}

public:
    virtual ~binder_base () {}

    size_t size () const
    {
        return _size;
    }

    virtual return_type operator () () const = 0;
};


template <typename Return>
class binder_function_base : public binder_base<Return>
{
public:
    typedef Return return_type;
    typedef return_type (* funcptr_type) ();

protected:
    funcptr_type _f;

    binder_function_base (size_t size, funcptr_type f)
        : binder_base<Return>(size)
        , _f(f)
    {}

public:
    virtual ~binder_function_base () {}

    virtual return_type operator () () const = 0;
};

template <typename Class, typename Return>
class binder_method_base : public binder_base<Return>
{
public:
    typedef Return return_type;
    typedef return_type (Class::* funcptr_type) ();

protected:
    Class * _p;
    funcptr_type _f;

    binder_method_base (size_t size, Class * p, funcptr_type f)
        : binder_base<Return>(size)
        , _p(p)
        , _f(f)
    { }

public:
    virtual ~binder_method_base () {}

    virtual return_type operator () () const = 0;
};

END_OF_CODE

my $footer = <<"END_OF_CODE";

} // pfs

#endif /* __PFS_BINDER_HPP__ */
END_OF_CODE


print $header, "\n";

for (my $i = 0; $i <= $numberOfParms; ++$i) {
	my $lines = binder_def($i, 0);
    print join "\n", @$lines;
    print "\n";
} 

# Generate binder for call class method
#
for (my $i = 0; $i <= $numberOfParms; ++$i) {
    my $lines = binder_def($i, 1);
    print join "\n", @$lines;
    print "\n";
} 

print $footer, "\n";

0;