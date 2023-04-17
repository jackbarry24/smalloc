# smalloc
Simple heap management library implementation.

Usage: 
Use the LD_PRELOAD trick to use our smalloc library instead of system malloc. This will load our implementation before any other libraries, overriding the symbols used by the system malloc.

1.  ```make```
2.  Pick ```First-Fit```, ```Worst-Fit``` or ```Best-Fit``` implementation
3.  Set LD_PRELOAD env: ```env LD_PRELOAD=lib/smalloc-{ff,wf,bf}.so```
4.  Compile 

