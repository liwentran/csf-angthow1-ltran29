Anthony Sky Ng-Thow-Hing - angthow1
Liwen Tran - ltran29

All functions were implemented through side-by-side partner coding for MS1 and MS2

Synchronization Report:

We concluded that Critical sections are the sections in our code in which data is accessed by multiple threads. The message_queue class is a perfect example of a class with critical sections. 
This is because several distinct threads could access the same message queue. Therefore, this could potentially cause issues as data in the queue could be modified through enqueues and
deques at the same time. This would in turn cause a data race where the distinct threads could be all doing reassignment at once in which consequentally could causes important data to be lost.
We were able to combat this issue through the usage of guards around all critical sections where code could be used in multiple threads to change variables accessed by all.
For example, aside from messagequeue, we also did this in room when adding and removing users because multiple threads can have access to the same rooms. 

Furthremore, we concluded that deadlocks happen when we have a sort of looping dependency. For example, X relies on Y and Y relies on Z and Z relies on X. This can occur when we have procceses
waiting for locks to be removed that may never unlocked. For example, a deadlock could occur in message_queue because we are using a semaphore to wait to see if there is a message to be dequed. 
To resolve this issue, we made sure to not have several guards active in the same process, as seen in message_queue. Guards are helpful though as the guard gets destroyed when it leaves scope. 
This is important because the mutex is unlocked and we do not have to manually do it. This helps prevent deadlocks overall. 


