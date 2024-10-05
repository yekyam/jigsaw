# Jigsaw - A simple test runner

## Description

Jigsaw compiles/runs the files in a given folder, saves the outputs, and reports the statuses (successes/failures).

Still new, doesn't work well, expect lots of changes.

## Usage

./jigsaw (path/to/folder)

## Sample output

```
********************Test Start********************
        -sample_fail.c          FAILED
        -fail_to_compile.c              FAILED
        -sample_pass.c          PASSED
********************[1/5] tests passed********************
```