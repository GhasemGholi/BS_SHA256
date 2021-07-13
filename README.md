# BS_SHA256
Bitsliced sha256 implementation and normal sha256 implementation and benchmark comparisons.

First make the build with ```make```

Then either do:
```make test_largeinput``` for benchmark the average of 1000 runs

```make run``` for benchmark one run

```make test_smallinput``` for benchmark the difference between Bitsliced sha256 and sha256 on single inputs.
