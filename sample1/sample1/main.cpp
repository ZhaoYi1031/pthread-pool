#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std;
#define NUM_THREADS    4
int jj=0;
class T{
public:
    int ZY = 0;
    T(T& a){
        cout << "Copy" << endl;
        this->ZY = a.ZY;
        
    }
    T(){
    }
    void work(){
        for (int i = 0; i < 10000; ++i){
            ZY++;
            jj++;
        }
    }
};
struct thread_data{
    int  thread_id;
    char *message;
    T* t;
};
void *PrintHello(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    my_data->t->work();
    //    printf("%d %d\n",my_data->thread_id, my_data->t);
    pthread_exit(0);
}
int main ()
{
    
    string s = "anc";
    for (auto c: s){
        cout << c << endl;
    }
    
    
    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];
    int rc;
    int i;
    
    int res = 0;
    
    T ttt[NUM_THREADS];
    printf("%d %d %d\n",  &ttt[0], &ttt[1], &ttt[2]);;
    for( i=0; i < NUM_THREADS; i++ ){
        printf("main() : creating thread, %d\n",  i);;
        td[i].thread_id = i;
        td[i].t = &ttt[i];//*(new T());//ttt[i];
        
        /*
         
         !!!
         
         不可以直接不用指针，td[i] = ttt[i];
         
         显然这里我们想要的td[i]指向ttt[i]的地方 以便td[i]加上ZY之后然后ttt[i]的ZY也跟上变化
         
         首先得意识到一点 T a; 这句话就已经分配给了a空间，相当于已经执行了构造函数。
         
         如果直接用td[i] = ttt[i] 呢，只是把当前ttt[i]里的值，也就是ZY的0给td[i].ZY，这一个默认拷贝构造函数做的事情。也就是浅拷贝。
         
         重写一个深拷贝也不行，因为重写一个深拷贝相当于新建一个内存给td[i]， 显然与我们的初衷不符。
         
         因此这个地方用指针是最好不过的，让td[i]指向ttt[i]的地址。
         
         */
        
        
        //        printf("%d %d %d\n",i, ttt[i],td[i].t);
        rc = pthread_create(&threads[i], NULL,
                            PrintHello, (void *)&td[i]); //传入到参数必须强转为void*类型，即无类型指针
        
        
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
        
        
        
    }
    for (int i = 0; i < NUM_THREADS; ++i){
        pthread_join(threads[i],NULL);
    }
    
    
    for (int i = 0; i < NUM_THREADS;  ++i)
        res+=td[i].t->ZY;//ttt[i].ZY;//td[i].t.ZY;
    cout << "res = " << res<< endl;
    //    pthread_exit(NULL);
    
    cout << "end " << endl;
    cout << "jj = "<< jj<<endl;
    return 0;
}
