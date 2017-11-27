#include <stdio.h> 
#include <stdlib.h> 



#ifdef _MSC_VER 
#include <Windows.h> 
#endif 

// define定義 
#define LEN 8 



#define WHITE 'O' //白 
#define BLACK 'X' //黒 
#define NONE ' ' 

//盤面
char field[LEN][LEN];

//向きごとの移動量
int vec_y[] = { -1,-1,0,1,1,1,0,-1 };
int vec_x[] = { 0,1,1,1,0,-1,-1,-1 };
//ターンを決める 
int turn = 0;

//フィールドの初期化
void setfield() {
	int x, y;


	for (y = 0; y < LEN; y++) {
		for (x = 0; x < LEN; x++) {
			field[y][x] = NONE;
		}
	}

	// 初期位置 
	field[LEN / 2 - 1][LEN / 2 - 1] = field[LEN / 2][LEN / 2] = WHITE;
	field[LEN / 2][LEN / 2 - 1] = field[LEN / 2 - 1][LEN / 2] = BLACK;


}


//盤面表示
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

//vec方向にひっくり返るコマが何個あるか確認する
int check(int x, int y, int vec) {
	int count = 0;
	while (1) {
		y += vec_y[vec];
		x += vec_x[vec];

		//盤面の外に出ていたら終了
		if (x < 0 || y < 0 || x > LEN - 1 || y > LEN - 1) return 0;

		//空きマスだったら終了
		if (field[y][x] == NONE) return 0;

		//相手のコマがあったら数を数える
		if (field[y][x] == (turn ? WHITE : BLACK)) {
			count ++;
			continue;
		}
		//自分の駒があったとき
		//もし相手の駒を挟んでいればループ脱出。いなければ終了
		if (count != 0) break;
		return 0;
	}
	return count;
}

//実際に裏返す
void flip(int x, int y, int vec) {
	while (1) {
		y += vec_y[vec];
		x += vec_x[vec];

		//自分のコマがあったら終了
		if (field[y][x] == (turn ? BLACK : WHITE)) break;

		//それ以外なら自分のコマで塗りつぶす
		field[y][x] = (turn ? BLACK : WHITE);
	}
}

//入力された場所に置くことができるか確認する
int put(int x, int y) {
	int vec;
	int f = 0;
	//コマがあったら再度入力
	if (field[y][x] != ' ') return 0;
	//上から時計回りに8方向について確認
	for (vec = 0; vec<8; vec++) {
		if (check( x, y, vec)>0 ) {
			flip( x, y, vec);
			f = 1;
		}
	}
	if (f) {
		//一方向でもひっくり返せたら入力位置にコマを置く
		field[y][x] = turn ? BLACK : WHITE;
		return 1;
	}
	return 0;
}



//field内の状況確認とCPUの動作 最も多く取れるところに置く
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



//結果表示
int result() {
	int i, j, b = 0, w = 0;
	int f = 0;
	//コマを数え上げる
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
		//最後に盤面表示
		printf("\n\n\n\n");
		printfield();

		//勝者を表示
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


	//スタート画面表示
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
	//各コマの画面表示
	printf("\n\n\n\n");
	printf("Player1 is ('%c') piece!\n", WHITE);
	printf(p_num == 2 ? "Player2 is ('%c') piece!\n" : "CPU is ('%c') piece!\n", BLACK);
	Sleep(2000);

	
	//ゲームループ
	while (1) {
		n_num = 0;
		printf("\n\n\n\n");

		//1人プレイ
		if (p_num == 1 && turn) {
			printf("CPU's('%c') turn:\n", BLACK);
			printfield();
			Sleep(2000);

			if (consider(1) == 0) {
				printf("CPU Pass...");
			}
		}
		//2人プレイ
		else {
			printf("Your('%c') turn:\n", turn ? BLACK : WHITE);
			printfield();


			if (consider(0) > 0) {
					while (1) {
						printf("Please input((example) x y ):  ");
						scanf_s("%d %d", &x, &y);
						//入力された場所に置くことができれば入力終了
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

		//置けるところがなければループを抜ける
		if (result() || n_num == 0  ) break;
		turn = (turn + 1) % 2;

	}


	return 0;
}
