#include <stdio.h> 
#include <stdlib.h> 



#ifdef _MSC_VER 
#include <Windows.h> 
#endif 

// define��` 
#define LEN 8 



#define WHITE 'O' //�� 
#define BLACK 'X' //�� 
#define NONE ' ' 

//�Ֆ�
char field[LEN][LEN];

//�������Ƃ̈ړ���
int vec_y[] = { -1,-1,0,1,1,1,0,-1 };
int vec_x[] = { 0,1,1,1,0,-1,-1,-1 };
//�^�[�������߂� 
int turn = 0;

//�t�B�[���h�̏�����
void setfield() {
	int x, y;


	for (y = 0; y < LEN; y++) {
		for (x = 0; x < LEN; x++) {
			field[y][x] = NONE;
		}
	}

	// �����ʒu 
	field[LEN / 2 - 1][LEN / 2 - 1] = field[LEN / 2][LEN / 2] = WHITE;
	field[LEN / 2][LEN / 2 - 1] = field[LEN / 2 - 1][LEN / 2] = BLACK;


}


//�Ֆʕ\��
void printfield() {
	int x, y;


	for (x = 0; x < LEN; x++) printf(x ? "  %d " : "     %d ", x);
	printf("\n");
	printf("   +---+---+---+---+---+---+---+---+\n");


	for (y = 0; y < LEN; y++) {
		printf(" %d |", y);
		for (x = 0; x < LEN; x++) {
			printf(x ? " %c |" : " %c |", field[y][x]);
		}
		printf("\n");
		printf("   +---+---+---+---+---+---+---+---+\n");
	}
	printf("\n");
}

//vec�����ɂЂ�����Ԃ�R�}�������邩�m�F����
int check(int x, int y, int vec) {
	int count = 0;
	while (1) {
		y += vec_y[vec];
		x += vec_x[vec];

		//�Ֆʂ̊O�ɏo�Ă�����I��
		if (x < 0 || y < 0 || x > LEN - 1 || y > LEN - 1) return 0;

		//�󂫃}�X��������I��
		if (field[y][x] == NONE) return 0;

		//����̃R�}���������琔�𐔂���
		if (field[y][x] == (turn ? WHITE : BLACK)) {
			count ++;
			continue;
		}
		//�����̋�������Ƃ�
		//��������̋������ł���΃��[�v�E�o�B���Ȃ���ΏI��
		if (count != 0) break;
		return 0;
	}
	return count;
}

//���ۂɗ��Ԃ�
void flip(int x, int y, int vec) {
	while (1) {
		y += vec_y[vec];
		x += vec_x[vec];

		//�����̃R�}����������I��
		if (field[y][x] == (turn ? BLACK : WHITE)) break;

		//����ȊO�Ȃ玩���̃R�}�œh��Ԃ�
		field[y][x] = (turn ? BLACK : WHITE);
	}
}

//���͂��ꂽ�ꏊ�ɒu�����Ƃ��ł��邩�m�F����
int put(int x, int y) {
	int vec;
	int f = 0;
	//�R�}����������ēx����
	if (field[y][x] != ' ') return 0;
	//�ォ�玞�v����8�����ɂ��Ċm�F
	for (vec = 0; vec<8; vec++) {
		if (check( x, y, vec)>0 ) {
			flip( x, y, vec);
			f = 1;
		}
	}
	if (f) {
		//������ł��Ђ�����Ԃ�������͈ʒu�ɃR�}��u��
		field[y][x] = turn ? BLACK : WHITE;
		return 1;
	}
	return 0;
}



//field���̏󋵊m�F��CPU�̓��� �ł���������Ƃ���ɒu��
int consider(int set) {
	int n_put[LEN][LEN] = { 0 };
	int x,y,vec, max = 0;

	for (x = 0; x < LEN; x++) {
		for (y = 0; y < LEN; y++) {
			if (field[y][x] == NONE) {
				for (vec = 0; vec < 8; vec++) {
					n_put[y][x] += check(x, y, vec);
				}
				if (n_put[y][x] > max) max = n_put[y][x];
			}
		}
	}
	if (max == 0)return max;
	do {
		x = rand() % 8;
		y = rand() % 8;
	} while (n_put[y][x] < max);
	if (set) {
		
		max = put(x, y);
		
	}
	return max;
}



//���ʕ\��
int result() {
	int i, j, b = 0, w = 0;
	int f = 0;
	//�R�}�𐔂��グ��
	for (i = 0; i < LEN; ++i) {
		for (j = 0; j < LEN; ++j) {
			switch (field[i][j]) {
			case BLACK:
				++b;
				break;
			case WHITE:
				++w;
				break;
			default:
				break;
			}
		}
	}
	if (b == 0 || w == 0) {
		f = 1;
		//�Ō�ɔՖʕ\��
		printf("\n\n\n\n");
		printfield();

		//���҂�\��
		if (b > w) printf("BLACK('%c') WINNER!!!!  WHITE('%c') LOSER...\n", BLACK, WHITE);
		else if (b < w) printf("WHITE('%c') WINNER!!!! BLACK('%c') LOSER...\n", WHITE, BLACK);
		else printf("Drawn Game.");
		Sleep(5000);
	}
	return  f;
}

int main(void) {

	
	int x = 0, y = 0;
	int n_num , p_num;


	//�X�^�[�g��ʕ\��
	printf("Welcome to Othello Game!!\n\n");
	setfield();
	turn = 0;
	printfield();

	printf("Player vs CPU: 1\nPlayer vs Player: 2\n\n");
	while (1) {
		printf("Please input a number: ");
		scanf_s("%d", &p_num);
		if (p_num == 1 || p_num == 2)break;
	}
	//�e�R�}�̉�ʕ\��
	printf("\n\n\n\n");
	printf("Player1 is ('%c') piece!\n", WHITE);
	printf(p_num == 2 ? "Player2 is ('%c') piece!\n" : "CPU is ('%c') piece!\n", BLACK);
	Sleep(2000);

	
	//�Q�[�����[�v
	while (1) {
		n_num = 0;
		printf("\n\n\n\n");

		//1�l�v���C
		if (p_num == 1 && turn) {
			printf("CPU's('%c') turn:\n", BLACK);
			printfield();
			Sleep(2000);

			if (consider(1) == 0) {
				printf("CPU Pass...");
			}
		}
		//2�l�v���C
		else {
			printf("Your('%c') turn:\n", turn ? BLACK : WHITE);
			printfield();


			if (consider(0) > 0) {
					while (1) {
						printf("Please input((example) x y ):  ");
						scanf_s("%d %d", &x, &y);
						//���͂��ꂽ�ꏊ�ɒu�����Ƃ��ł���Γ��͏I��
						if (put(x, y)) 	break;
						}
					}
					else {
						printf("You Pass...");
					}
				}
			
		for (x = 0; x < LEN; x++) {
			for (y = 0; y < LEN; y++) {
				if (field[y][x] == NONE) {
					n_num++;
				}
			}
		}

		//�u����Ƃ��낪�Ȃ���΃��[�v�𔲂���
		if (result() || n_num == 0  ) break;
		turn = (turn + 1) % 2;

	}


	return 0;
}
