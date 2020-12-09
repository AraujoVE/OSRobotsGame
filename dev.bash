#!/bin/bash
target=gmake2
err=0

sys_command=premake5
shipped_bin=vendor/premake5/binaries/premake5

premake_success_postcommands='make -j all && ./bin/Debug*/Application/Application'
premake_succes=0

($sys_command $target || 
    (
        echo "" &&
        echo "WARNING: Premake5 is not installed on the system, searching for shipped precompiled binary" &&
        echo "" &&
        $shipped_bin $target 
    )
) && (premake_succes=1 && echo "> Premake executed with 0 errors" && eval $premake_success_postcommands) || 
    (   
        err=$?;

        [[ err > 0 ]] && echo "*** ERRORS HAVE OCCURRED! ***" &&
        [[ "$premake_succes" = 1 ]] && echo "But premake5 executed flawlessly" exit $err;

        echo "";

        [[ "$err" = 127 ]] && echo "ERROR: couldn't find vendor/premake5/binaries/premake5 (it should come with the git repository)"
        printf "\nPremake FAILED :( status=%d\n" $err || 
        echo "Premake Failed :( status=$err";
        exit $err
    )