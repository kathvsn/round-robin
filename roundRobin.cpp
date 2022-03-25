#include <iostream>
#include <stdio.h>
#include <list>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
using namespace std;

// used to interrupt/wake up a process
const int TIMESLICE = 3;
// determines if the alarm has been triggered to wake up a process
bool alarmFired = false;

/*
* struct Node
* creating a linked list where each element is a Node that has data that contains the value
* to be stored in the node, and a pointer to the next node on the list
*/

struct Node{
    string data;
    Node *next;
};

/*
 * void enqueueName(Node** front, string name)
 *  Insert the name of a process at the end of the queue. If a process still has a remaining burst time, its
 *  process name is enqueued again so that the process runs again.
 * @param front: a pointer to a pointer to a node object, that is the front of the process name queue
 * @param name: name of a process
 */

void enqueueName(Node** front, string name){
    // create a new node whose data holds the name of a process and set the next pointer of the node to NULL
    Node* nameNode = new Node();
    nameNode->data = name;
    nameNode->next = NULL;

    // if the queue is empty, set the front of the queue to be the new node
    if (*front == NULL) {
        *front = nameNode;
    }
    else{
        // traverse from the front node to the rear node and insert the new node after the rear node
        Node *rear = *front;
        while (rear->next != NULL){
            rear = rear->next;
        }
        rear->next = nameNode;
    }
    return;
}

/*
 * void enqueueBurst(Node** front, string burst)
 *  Insert the burst time of a process at the end of the queue. If a process still has a remaining burst time, its burst
 *  time is enqueued again so that the process runs again.
 * @param front: a pointer to a pointer to a node object, that is the front of the process burst time queue
 * @param burst: burst time of a process
 */

void enqueueBurst(Node** front, string burst){
    // create a new node whose data holds the burst time of a process and set the next pointer of the node to NULL
    Node* burstNode = new Node();
    burstNode->data = burst;
    burstNode->next = NULL;

    // if the queue is empty, set the front of the queue to be the new node
    if (*front == NULL) {
        *front = burstNode;
    }
    else{
        // traverse from the front node to the rear node and insert the new node after the rear node
        Node *last = *front;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = burstNode;
    }

    return;
}

/*
 * void dequeueProcess(Node** frontName, Node** frontBurst)
 *  If a process still has a remaining burst time, it is removed from the front of the queue so that it may run again
 * @param frontName: a pointer to a pointer to a node object, that is the front of the process name queue
 * @param frontBurst: a pointer to a pointer to a node object, that is the front of the process burst time queue
 */
void dequeueProcess(Node** frontName, Node** frontBurst){
    Node *temp1, *temp2;
    // if the queues are empty there are no processes left
    if(*frontName == NULL || *frontBurst == NULL){
        cout << "No processes left" << endl;
    }
    else{
        // have temporary nodes hold the front of the queues then reassign the front nodes to the next node
        // delete the temporary nodes
        temp1 = *frontName;
        temp2 = *frontBurst;
        *frontName = temp1->next;
        *frontBurst = temp2->next;
        delete temp1;
        delete temp2;
    }
    return;
}

/*
 * void sigalarm_handler(int signum)
 *  User defined signal handler for the signal number that is registered
 *  using signal(unsigned int signum, sighandler_t)
 * @param signum: signal number of the signal that caused the handler to fire.
 */

void sigalarm_handler(int signum){
    // a signal handler used to simulate the CPU timer that goes off when the system has to schedule a process
    alarmFired = true;
}

/*
 * void roundRobinSimulator(Node *node1, Node *node2)
 *  Function that demonstrates the round robin scheduling method using a signal
 *  and alarm system to simulate the running of processes.
 *  @param node1: Process name
 *  @param node2: Integer time to indicate burst
 */

void roundRobinSimulator(Node *node1, Node *node2){
    // register signal handler
    signal(SIGALRM, sigalarm_handler);
    cout << endl;

    // until the queues are empty, print out the process name and its remaining burst time
    while(node1 != NULL || node2 != NULL){
        // scheduled alarm for 3 seconds
        alarm(TIMESLICE);
        // convert burst time from a string to an integer and use it in the sleep system call to simulate process running
        int burst = stoi(node2->data);
        sleep(burst);
        // newRemain is the remaining time the process has left after being awoken by the alarm
        string newRemain;
        // if a burst is greater than 3 and a process has been awoken by the alarm, subtract 3
        // reassign the new time remaining for that process and at it to the end of the queue
        if(burst > TIMESLICE && alarmFired == true){
            burst = burst - TIMESLICE;
            newRemain = to_string(burst);
            node2->data = newRemain;
            enqueueName(&node1, node1->data);
            enqueueBurst(&node2, node2->data);
        }
        else if(burst <= TIMESLICE){
            // if the burst is less than 3, the process will be finished
            node2->data = "Finished";
        }
        // print out process name and burst time
        cout << node1->data << " " << node2->data << endl;
        // dequeue the process at the front of the process name and burst time queues
        dequeueProcess(&node1, &node2);
        // reset alarmFired
        alarmFired = false;
    }
    // if the queue is empty there are no processes left to run
    if(node1 == NULL){
        cout << "No processes left" << endl;
    }
}


int main(void){
    // process contains both the process name and the burst time
    string process;
    string processName = "";
    string burstTime = "";

    // two queues implemented as linked lists
    // processQueue holds data for the process names and timeQueue holds data for the burst time for the corresponding process
    Node *processQueue = NULL;
    Node *timeQueue = NULL;

    // read in input until end of transmission
    while (getline(cin, process)){
        // split string input into process name and burst time by whitespace
        for (int i = 0; i < process.length(); i++){
            // if the current char is a whitespace, it means that the process name has been formed and should be separated
            if(process[i] == ' '){
                // insert process name as a node into the process queue and reset the process name string
                enqueueName(&processQueue, processName);
                processName = "";
            }
            else if(isdigit(process.at(i))){
                // if the char is a digit, create a new string to form the burst time
                burstTime = burstTime + process[i];
            }
            else{
                // form a string for the process name
                processName = processName + process[i];
            }
        }
        // insert burst time as a node into the time queue and reset the burst time string
        enqueueBurst(&timeQueue, burstTime);
        burstTime = "";
    }
    // begin the round robin simulator
    roundRobinSimulator(processQueue, timeQueue);
    return 0;
}