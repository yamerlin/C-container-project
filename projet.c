#define _GNU_SOURCE //Récupérer les flags pour unshare()

#include <stdio.h> //Librairies standard
#include <stdlib.h>
//#include <sys/stat.h>
#include <sched.h> //Librairie pour unshare() ??? source https://man7.org/linux/man-pages/man2/unshare.2.html mais la fonction unshare() est quand même reconnu même sans cette librairie
//include <sys/types.h>
#include <unistd.h> //Lib pour fork()

//int unshare(int flags);

int main()
{
	//Avant le fork, le processus doit appeler
	//unshare afin de créer un nouveau namespace
	//processus.

	int returnedValueUnshare = unshare(CLONE_NEWPID); //Unshare(CLONE_NEWPID) fail si tu execute pas le binaire avec sudo

	if(returnedValueUnshare == -1){
		printf("Unshare echec \n");
		printf("Etes vous un utilisateur root ? (sudo) \n");
		exit(1);
	}
	else if(returnedValueUnshare == 0){
	 	printf("Unshare reussi \n");
	}

	//char pids[] = "";

	//system("echo $$");

	//printf("%s", pids);


	//Le processus se fork
	// Succès --> returnedValueFork = 0 dans l'enfant
	//			  returnedValueFork = PID de l'enfant dans le parent
	// Echec  --> returnedValueFork = -1 dans le parent
	//            pas d'enfant
	
	int returnedValueFork = fork();

	if(returnedValueFork == -1){
		printf("Fork echec \n");
	}
	else if(returnedValueFork == 0){
	 	printf("Fork reussi \n");
	}

	//Verifier que l'on est dans le processus enfant (returnedValueFork = 0)
	if(returnedValueFork == 0){
		printf("Nous sommes dans le processus enfant \n");

		int pid = getpid();
		printf("PID de l'enfant : %d", pid);
		printf("\n");

		//Créer les autres namespaces
		returnedValueUnshare = unshare(CLONE_NEWNS); //Systeme de fichier
		if(returnedValueUnshare == -1){
			printf("Echec de la creation d'un nouveau namespace de systeme de fichier\n");
			exit(1);
		}
		else if(returnedValueUnshare == 0){
			printf("Reussite de la creation d'un nouveau namespace de systeme de fichier\n");
		}
		// unshare(CLONE_NEWCGROUP); 	//cgroup
		// unshare(CLONE_NEWIPC);		//ipc
		// unshare(CLONE_NEWNET);		//net
		// unshare(CLONE_NEWUSER);		//user
		// unshare(CLONE_NEWUTS);		//uts

		//Le processus fils effectue un chroot
		//Les dossiers bin, lib, lib64, tmp, proc et media
		//ont été créés au préalable dans /home/yann/Cprojects
		// A l'aide des commandes suivantes :
		// mkdir bin
		// mkdir lib
		// mkdir lib64
		// mkdir tmp
		// mkdir proc
		// mkdir media
		// cp bash /home/yann/Cprojects/bin/
		// cp ps /home/yann/Cprojects/bin/
		// cp kill /home/yann/Cprojects/bin/
		// cp ls /home/yann/Cprojects/bin/
		// cp /lib/x86_64-linux-gnu/ lib
		// cp /lib/x86_64-linux-gnu/libtinfo.so.6 lib
		// cp /lib/x86_64-linux-gnu/libc.so.6 lib
		// cp /lib64/ld-linux-x86-64.so.2 lib64


		char path[] = "/home/yann/Cprojects";
		int returnedValueChroot = chroot(path);
		if(returnedValueChroot == -1){
			printf("Echec du chroot\n");
			exit(1);
		}
		else if(returnedValueChroot == 0){
			printf("Reussite du chroot\n");
		}
		

		//Le processus fils recouvre son code avec celui du bash
		int returnValueExeclp = execlp("/bin/bash","bash",NULL);
		if(returnValueExeclp == -1){
			printf("Echec de l'execution du bash");
		}
	}

	printf("\n");
	return 0;
}

