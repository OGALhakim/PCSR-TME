#include <iostream>

pid_t pid;
volatile sigatomic_t PV = 3;

void attaque(pid_t adversaire){
struct sigaction sa;
    sa.sa_handler = attacque_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1,&sa,nullptr) <0){
        perror("sigaction");
        exit(1);
    }
    if (kill(adversaire,SIGUSR1)){
        exit(0);//t'as gagner beau-gosse 
    }
    rsleep();
}
void defense(){
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1,&sa,nullptr) <0){
        perror("sigaction");
        exit(1);
    }
    rsleep();
}

void combat(pid_t adversaire){

}

int main(arg c, char **argv){
    int T = atoi (argv[1]);
    
    //handler_ALARM_je le remplis apres
    
    
    pid =fork();
    if(pid < 0){perror("fork"); return 2;}
    else if (pid == 0) { 
        //Luke
        combat(getppid);
    } else {
        //Vador
        combat(getpid);

    }

}