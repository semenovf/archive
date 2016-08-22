###NAME

`pfs-test` - unit test library.

###VERSION

Version 1.00

###SYNOPSIS

    int main(int argc, char *argv[])  
    {  
        BEGIN_TESTS(116);  
        
        TEST_OK(true == true);  
        TEST_OK2(true == true, "True is true");
        TEST_FAIL(true == true); // Breaks tests if result of expression is false  
        TEST_FAIL2(true == true, "Test failure");  
        
        return END_TESTS;  
    }

###AUTHOR

Fedor Semenov, < fedor.v.semenov at gmail.com >

###LICENSE AND COPYRIGHT

Copyright 2014-2016 Fedor Semenov.  
