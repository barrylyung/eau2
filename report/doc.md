**Milestone 1**

Nick Simmons and Barry Yung

**Introduction:**

Eau2 is a distributed system consisting of a master node with N number of supporting compute nodes. The network will be capable of storing tabular dataframe objects of up to around 100 columns and in the billions of rows. The distributed system will read data once. Upon constructing the dataframe object, the network will handle distributing subsets of the dataframe around the cluster to allow the overlaying user applications to process queries in-memory. Users will be able to take advantage of two lower layers of abstraction in order to construct whatever application their hearts desire. 


**Architecture:**

The eau2 system architecture consists of a system designed in three layers. At the bottom layer, the eau2 system holds a distributed key/value store running on each node. Each key/value store, or node within the network, contains a portion of the actual dataframe. The master node keeps track of which key/value store owns which subsets of the dataframe. The key/value stores are capable of communicating with each other in order to exchange data as needed. This bottom layer controls all of the networking protocols and abstracts the in-memory storing of our users’ data. The second layer provides abstractions for distributed arrays and data frames. Such objects will be populated by the data housed in the network cluster of key/value stores. This middle layer provides an abstraction for retrieving data from the underlying key/value storing system and constructing usable objects for our top layer user applications. Lastly, the top layer of the eau2 system is the application layer. The application layer allows our customers/users to write whatever code that they would like and provides them full functional access over the dataframe stored in the underlying distributed key/value store system. 


**Implementation:**
**Relevant classes/files:**

●	*Server* - This object is the master node of the distributed network. A server holds a master list of the key/values that make up the entire dataframe.

●	*Client* - This object is a key/value store node. Multiple of these can exist within the distributed network. Each client object will be responsible for holding a portion of the overall dataframe.

●	*ModifiedDataFrame* - The object is the dataframe object that will be split up and distributed over the network. It is also the dataframe object that will be used by the user applications and is capable of storing four types of data (String, Integer, Boolean, and Float) in a column-wise format.

●	*Schema* - This object is responsible for describing a dataframe object. A schema object is essential to a dataframe object as it sets and upholds the integrity of each column type. Each dataframe object requires a single schema.

●	*Column* - This object makes up a dataframe object. There are four distinct types of columns: String column, Integer column, Boolean column, and Float column. Column objects are constructed from distributed arrays and are capable of serializing/deserializing themselves. 

●	*Message* - This object specifies each type of message that can be sent around the distributed network. Currently supported messages include: Ack, Nack, Put, Reply, Get, WaitAndGet, Status, Kill, Register, Directory. Each type of message has its own subclass and is capable of transforming itself into a sendable string message. 

●	*serial.h* - This file holds a series of serial supporting functions. Some functions include: serialize, deserialize, getTagValue, printAsBytes, and others. These functions are primarily used to support the more specific subclasses of Message. These messages will transform themselves into sendable Strings and then functions in this file will serialize such Strings into sendable bytes and later deserialize received bytes into Strings that can be converted into their actual types.

●	*Rower* - This object serves as the dataframe’s querying object. The rower class is subclassed by more specific querying classes and performs such queries on each row of a dataframe. 

●	*Application* - The application object is a parent object for more specific user defined applications. These application subclasses will take advantage of the dataframe stored in the distributed network in order to produce desired queries and run an endless set of compatible commands. 





**Use Cases:**

    class Demo : public Application {
    public:
      Key main("main",0);
      Key verify("verif",0);
      Key check("ck",0);
 
      Demo(size_t idx): Application(idx) {}
 
      void run_() override {
        switch(this_node()) {
        case 0:   producer();     break;
        case 1:   counter();      break;
        case 2:   summarizer();
      }
    }

      void producer() {
        size_t SZ = 100*1000;
        double* vals = new double[SZ];
        double sum = 0;
        for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
        ModifiedDataFrame::fromArray(&main, &kv, SZ, vals);
        ModifiedDataFrame::fromScalar(&check, &kv, sum);
      }
 
      void counter() {
        ModifiedDataFrame* v = kv.waitAndGet(main);
        size_t sum = 0;
        for (size_t i = 0; i < 100*1000; ++i) sum += v->get_double(0,i);
        p("The sum is  ").pln(sum);
        ModifiedDataFrame::fromScalar(&verify, &kv, sum);
      }
 
      void summarizer() {
        ModifiedDataFrame* result = kv.waitAndGet(verify);
        ModifiedDataFrame* expected = kv.waitAndGet(check);
        pln(expected->get_double(0,0)==result->get_double(0,0) ? "SUCCESS":"FAILURE");
      }
    };




**Open Questions:**

We are slightly confused about the sequence in which the eau2 system spins up. We are confident that the distributed key/value storing network needs to exist prior to a user executing an application. However, we are unsure if networks are unique to applications, i.e. a network spins up specifically for an application, or if multiple applications will be utilizing a single network of data. To further this point, where is the tabular data provided? Does each user application need to provide the underlying data set that it would like to operate on or are applications expected to run on top of preloaded, yet still relevant, data sets? 

**Status:**

  We currently have a functional network, capable of direct node to node communication, node to server communication, and an organized system teardown. We still need to implement a key/value storing structure within each node, a master key/value structure within the server, and a communication protocol set for distributing the entire dataframe. These additions should take around a week. Since our nodes are already capable of storing information about each other, updating information about each other and sending large payloads around the cluster, we simply need to make these features more specific rather than implementing new functionalities.  
	
  Our dataframe currently supports columns of the four required types, resizes itself, and performs complex queries in a reasonable amount of time. Our dataframe object supports self serialization/deserialization and therefore is already capable of being transported across the network. Currently, our dataframe still needs the key/value logic required for distributing itself across, and recreating itself from, the network cluster. This should take around half a week to implement since our dataframe is currently constructed via distributed array columns.
	
  Lastly, we need to create a powerful application object. This object will be subclassed by user-desired functionalities and capable of utilizing all data living within the abstracted eau2 system. We have not implemented the application object, or any of its subclasses. We anticipate it taking around a week to complete the application object, along with one or two functional user subclasses. 
 
