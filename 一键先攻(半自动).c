#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 100

char monster[MAX_LENGTH] = {0};
char input[MAX_LENGTH] = {0};
char continue_choice = '\0';
int num, i, j , k, l, m;
int category; 
int initiative, health, modifiers;//创建先攻，HP，调整值量 
int constitution, dexterity; //引入属性值 
int dexterity_modifiers, constitution_modifiers;//引入调整值 
int dice_num, dice_face;//引入骰数以及骰面 

typedef struct {
    char *name;
    int dexterity;
    int constitution;
    int initiative;
    int health;
    int dexterity_modifiers;
    int constitution_modifiers;
    int dice_num;
	int dice_face; 
} Monster;


typedef struct{
	char *names;
	int dexterities;
	int dexterity_modifiers;
	int initiatives;
} Character; 


int roll_initiative(int a);//计算先攻的函数 
int roll_health(int a,int b,int c);//计算血量的函数 
int ex_modifiers(int ability); //修正调整值的函数 
void clear_input_buffer(); 

int main(){
	srand(time(0));
	int pattern, total_monsters;
	
	//分别为玩家和怪物求先攻 
	printf("请选择模式(1为怪物模式，2为玩家模式)\n");
	scanf("%d",&pattern); 
	
	while ((getchar())!= '\n');//清理标准输入缓冲区，确保后续fgets能正常工作 
	
	switch (pattern){
		case 1:
		{
	        printf("请输入怪物种类数\n");
	       	while (scanf("%d", &category)!= 1 || category <= 0) {
				void clear_input_buffer();
				printf("输入错误，请输入一个正整数作为怪物种类数：\n");
			}
	        void clear_input_buffer();
	        int index = 0;
	        Monster *monsters = (Monster *)malloc(category * sizeof(Monster));
	        Monster *all_monsters = NULL;
	        int all_monsters_size = 0; 
	        
	        for (i = 0; i < category; i++) {
	            monsters[i].name = (char *)malloc(MAX_LENGTH * sizeof(char));
	            if (monsters[i].name == NULL) {
			        printf("内存分配失败，无法为怪物名称分配内存！\n");
			        return 1;
			    }
	            printf("请输入第%d种怪物名称、数量，全部数据输入完后再按回车键结束输入：\n", i+1);
	            scanf("%s %d", monsters[i].name, &num);
	            void clear_input_buffer();
	            
	            
	            printf("输入%s怪物的敏捷属性值、体质属性值、骰数和骰面(各值之间用空格隔开)\n", monsters[i].name);
		        scanf("%d %d %d %d", &monsters[i].dexterity, &monsters[i].constitution, &monsters[i].dice_num, &monsters[i].dice_face);
				void clear_input_buffer();
				
				monsters[i].dexterity_modifiers = ex_modifiers(monsters[i].dexterity);
		        monsters[i].constitution_modifiers = ex_modifiers(monsters[i].constitution);
		        
	            for(j = 0; j < num; j++){	
		            monsters[i].initiative = roll_initiative(monsters[i].dexterity_modifiers);
		            monsters[i].health = roll_health(monsters[i].dice_num, monsters[i].dice_face, monsters[i].constitution_modifiers);
					
			        printf("%s[%d]的先攻为:%d，血量为:%d\n", monsters[i].name, j + 1, monsters[i].initiative, monsters[i].health);
					
					if (index >= all_monsters_size) {
			            all_monsters_size += num; // 根据当前怪物数量适当增加可容纳数量，可根据实际情况调整增长策略
			            all_monsters = (Monster *)realloc(all_monsters, all_monsters_size * sizeof(Monster));
			            if (all_monsters == NULL) {
			                printf("内存分配失败，无法为all_monsters重新分配内存！\n");
			                return 1;
			            }
			        }
					all_monsters[index].name = (char *)malloc(strlen(monsters[i].name) + 1);
					if (all_monsters[index].name == NULL) {
					    printf("内存分配失败，无法为all_monsters的name字段分配内存！\n");
					    return 1;
					}
                	strcpy(all_monsters[index].name, monsters[i].name);
					
					all_monsters[index].dexterity = monsters[i].dexterity;
				    all_monsters[index].constitution = monsters[i].constitution;
				    all_monsters[index].dexterity_modifiers = monsters[i].dexterity_modifiers;
				    all_monsters[index].constitution_modifiers = monsters[i].constitution_modifiers;
				    all_monsters[index].dice_num = monsters[i].dice_num;
				    all_monsters[index].dice_face = monsters[i].dice_face;

	                all_monsters[index].initiative = monsters[i].initiative;
	                all_monsters[index].health = monsters[i].health;
	                index++;
				}
			}
	
	        // 冒泡排序，对所有怪物个体按照先攻值从小到大排序
	        for (k = 0; k < all_monsters_size - 1; k++) {
	            for (l = 0; l < all_monsters_size - k - 1; l++) {
	                if (all_monsters[l].initiative > all_monsters[l + 1].initiative) {
	                    Monster temp = all_monsters[l];
	                    all_monsters[l] = all_monsters[l + 1];
	                    all_monsters[l + 1] = temp;
	                    
	                    char *temp_name_l = all_monsters[l].name;
			            all_monsters[l].name = (char *)malloc(strlen(all_monsters[l + 1].name) + 1);
			            strcpy(all_monsters[l].name, all_monsters[l + 1].name);
			
			            all_monsters[l + 1].name = (char *)malloc(strlen(temp_name_l) + 1);
			            strcpy(all_monsters[l + 1].name, temp_name_l);
			
			            free(temp_name_l);
			            all_monsters[l + 1] = temp;
	                }
	            }
	        }
	
	        // 输出排序后的所有怪物个体信息
	        printf("排序后的怪物信息：\n");
	        for (i = 0; i < all_monsters_size; i++) {
	            printf("怪物%s[%d]的先攻为:%d，血量为:%d\n", all_monsters[i].name, i + 1, all_monsters[i].initiative, all_monsters[i].health);
	        }
	       
	        // 释放动态分配的内存
	        for (i = 0; i < num; i++) {
	            free(monsters[i].name);
	        }
	        free(monsters);
	        for (i = 0; i < total_monsters; i++) {
	            free(all_monsters[i].name);
	        }
	        free(all_monsters);
	
	    } 
        break;

		case 2:
    		{
    		printf("请输入玩家数量\n");
    		scanf("%d", &num);
    		// 清空输入缓冲区，防止用户输入玩家数量后误输入换行符等影响后续fgets
    		void clear_input_buffer();

    		Character *characters = (Character *)malloc(num * sizeof(Character));
    		for (i = 0; i < num; i++) {
        		characters[i].names = (char *)malloc(MAX_LENGTH * sizeof(char));
    		}

    		// 提示输入所有玩家的名称和敏捷属性值的格式要求
    		printf("请按顺序依次输入所有玩家的名称和敏捷属性值（各值之间用空格隔开），全部数据输入完后再按回车键结束输入：\n");

    		// 一次性获取所有玩家的输入
    		fgets(input, sizeof(input), stdin);

    		// 初始化索引用于后续解析
    		int inputIndex = 0;

    		// 解析所有玩家的输入
    		for (i = 0; i < num; i++) {
        		// 确保输入未超出范围且当前字符不是换行符
        		while (inputIndex < strlen(input) && input[inputIndex]!= '\n') {
            		sscanf(&input[inputIndex], "%s %d", characters[i].names, &characters[i].dexterities);

            		// 更新输入索引，跳过已解析的部分
            		inputIndex += strlen(characters[i].names) + strlen(" ") + strlen("%d") + 1;

            		// 计算调整值
            		characters[i].dexterity_modifiers = ex_modifiers(characters[i].dexterities);

            		// 计算先攻值
            		characters[i].initiatives = roll_initiative(characters[i].dexterity_modifiers);
				
					// 冒泡排序玩家结构体数组按照先攻值从小到大排序
					for (k = 0; k < num - 1; k++) {
						for (j = 0; j < num - k - 1; j++) {
    						if (characters[j].initiatives > characters[j + 1].initiatives) {
        						// 交换结构体
        						Character temp = characters[j];
        						characters[j] = characters[j + 1];
        						characters[j + 1] = temp;
    						}
						}
					}
        		}
		
				// 输出相关信息
				for(i=0; i < num; i++){
        			printf("玩家 %s 的先攻加值为%d\n", characters[i].names, characters[i].dexterity_modifiers);
        			printf("%s的先攻为:%d\n", characters[i].names, characters[i].initiatives);
				}
        		// 跳过换行符，准备解析下一个玩家的数据
				if (inputIndex < strlen(input)) {
    				inputIndex++;
				}
			}		

    		// 释放动态分配的内存
    		for (i = 0; i < num; i++) {
       			free(characters[i].names);
    		}
    		free(characters);
		}
		break;
    		
		default :
			printf("error!please try again.\n") ;
	}
	
	return 0; 
	
}

//计算调整值，并消除由于负数舍入与调整值计算不符造成的错误 
int ex_modifiers(int ability){
	if(ability >= 10 || ability % 2 ==0)
		return (ability - 10) / 2;
	else
		return (ability - 10) / 2 - 1;
}


int roll_initiative(int a){
	return rand() % 20 + 1 + a;
}

int roll_health(int a,int b,int c){
	int total = 0;
	int i = 0;
    for (i = 0; i < a; i++) {
        // 生成每个骰子的随机点数，范围是1到骰子面数
        int roll = rand() % b + 1;
        total += roll;
    }
    total = total + a * c;
    return total;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar())!= '\n' && c!= EOF);
}