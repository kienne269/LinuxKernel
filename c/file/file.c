#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void list_file(int argc, char *argv[]) {
	char dir_path[100];
	strcpy(dir_path, ".");
	DIR *d;
	struct dirent *dir;

	if (argc == 3) {
		strcpy(dir_path, argv[2]);
	}
	d = opendir(dir_path);
	if (d) {
		while ( (dir = readdir(d)) != NULL) {
			printf("%s\n", dir->d_name);
		}
		closedir(d);
	} else {
		printf("folder khong ton tai\n");
		exit(0);
	}
}

void create_file(int argc, char *argv[], char *str) {
	int fd;

	fd = open(str, O_CREAT, 0666);
	if (fd == -1) {
		printf("tao file that bai\n");
		exit(0);
	} else {
		printf("tao file %s thanh cong\n", str);
		exit(0);
	}
}

void remove_file(int argc, char *argv[], char *str) {
	int status;

	status = unlink(str);

	if (status == -1) {
		printf("Ten file sai hoac khong ton tai\n");
		exit(0);
	} else {
		printf ("Xoa file %s thanh cong \n", str);
		exit(0);
	}
}

void cat_file(int argc, char *argv[], char *str) {
	int fd;
	char buffer[1000];
	
	fd = open(str, O_RDONLY);
	if (fd == -1) {
		printf("Khong tim thay file muon doc\n");
		exit(0);
	}

	while (read(fd, buffer, sizeof(buffer)) != 0) {
		printf("%s", buffer);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	int luaChon;
	char str[20];
	
	do {
		printf("=============== MENU ===============");
		printf("\n1. Hien thi danh sach file");
		printf("\n2. Doc file");
		printf("\n3. Tao file");
		printf("\n4. Sua file");
		printf("\n5. Xoa file");
		printf("\n0. Thoat.");
		printf("\nBan chon ? ");
		
		scanf("%d", &luaChon);
		
		switch(luaChon) {
			case 0:
				break;
			
			case 1:
				printf("\nHien thi danh sach file :\n");
				list_file(argc, argv);
				break;

			case 2:
				printf("\nChon file muon doc:\n");
    			scanf("%s", str);
				cat_file(argc, argv, str);
				break;
				
			case 3:
				printf("\nChon ten file :\n");
    			scanf("%s", str);
				create_file(argc, argv, str);
				break;
				
			case 4:
				printf("\nChon file can chinh sua :\n");
				scanf("%s", str);
				break;
				
			case 5:
				printf("\nChon file can xoa:\n");
				scanf("%s", str);
				remove_file(argc, argv, str);
				break;
				
			default:
				printf("Sai chuc nang, vui long chon lai!\n");
				break;
		}
		
	} while(luaChon);
	
	return 0;
}