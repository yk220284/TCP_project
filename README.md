
# Start Server and Client
Open two terminals on a computer and run the following commands.
```bash
make server
make client
# Start server before starting client.
./server 220285
# In a different terminal.
./client localhost 220285 
```
Note `localhost` can be changed to other valid alias and `220285` can be changed to other available port number.

# See more function of `Portfolio` class 
Run below.
```bash
make 
./main
```

# TODO 
1. I didn't have time to write Unit Tests.
2. I didn't have time to write function that defferentiates message according to information in the header  (this should be a helper function in the header class).
