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
int initiative, health, modifiers;//�����ȹ���HP������ֵ�� 
int constitution, dexterity; //��������ֵ 
int dexterity_modifiers, constitution_modifiers;//�������ֵ 
int dice_num, dice_face;//���������Լ����� 

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


int roll_initiative(int a);//�����ȹ��ĺ��� 
int roll_health(int a,int b,int c);//����Ѫ���ĺ��� 
int ex_modifiers(int ability); //��������ֵ�ĺ��� 
void clear_input_buffer(); 

int main(){
	srand(time(0));
	int pattern, total_monsters;
	
	//�ֱ�Ϊ��Һ͹������ȹ� 
	printf("��ѡ��ģʽ(1Ϊ����ģʽ��2Ϊ���ģʽ)\n");
	scanf("%d",&pattern); 
	
	while ((getchar())!= '\n');//�����׼���뻺������ȷ������fgets���������� 
	
	switch (pattern){
		case 1:
		{
	        printf("���������������\n");
	       	while (scanf("%d", &category)!= 1 || category <= 0) {
				void clear_input_buffer();
				printf("�������������һ����������Ϊ������������\n");
			}
	        void clear_input_buffer();
	        int index = 0;
	        Monster *monsters = (Monster *)malloc(category * sizeof(Monster));
	        Monster *all_monsters = NULL;
	        int all_monsters_size = 0; 
	        
	        for (i = 0; i < category; i++) {
	            monsters[i].name = (char *)malloc(MAX_LENGTH * sizeof(char));
	            if (monsters[i].name == NULL) {
			        printf("�ڴ����ʧ�ܣ��޷�Ϊ�������Ʒ����ڴ棡\n");
			        return 1;
			    }
	            printf("�������%d�ֹ������ơ�������ȫ��������������ٰ��س����������룺\n", i+1);
	            scanf("%s %d", monsters[i].name, &num);
	            void clear_input_buffer();
	            
	            
	            printf("����%s�������������ֵ����������ֵ������������(��ֵ֮���ÿո����)\n", monsters[i].name);
		        scanf("%d %d %d %d", &monsters[i].dexterity, &monsters[i].constitution, &monsters[i].dice_num, &monsters[i].dice_face);
				void clear_input_buffer();
				
				monsters[i].dexterity_modifiers = ex_modifiers(monsters[i].dexterity);
		        monsters[i].constitution_modifiers = ex_modifiers(monsters[i].constitution);
		        
	            for(j = 0; j < num; j++){	
		            monsters[i].initiative = roll_initiative(monsters[i].dexterity_modifiers);
		            monsters[i].health = roll_health(monsters[i].dice_num, monsters[i].dice_face, monsters[i].constitution_modifiers);
					
			        printf("%s[%d]���ȹ�Ϊ:%d��Ѫ��Ϊ:%d\n", monsters[i].name, j + 1, monsters[i].initiative, monsters[i].health);
					
					if (index >= all_monsters_size) {
			            all_monsters_size += num; // ���ݵ�ǰ���������ʵ����ӿ������������ɸ���ʵ�����������������
			            all_monsters = (Monster *)realloc(all_monsters, all_monsters_size * sizeof(Monster));
			            if (all_monsters == NULL) {
			                printf("�ڴ����ʧ�ܣ��޷�Ϊall_monsters���·����ڴ棡\n");
			                return 1;
			            }
			        }
					all_monsters[index].name = (char *)malloc(strlen(monsters[i].name) + 1);
					if (all_monsters[index].name == NULL) {
					    printf("�ڴ����ʧ�ܣ��޷�Ϊall_monsters��name�ֶη����ڴ棡\n");
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
	
	        // ð�����򣬶����й�����尴���ȹ�ֵ��С��������
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
	
	        // ������������й��������Ϣ
	        printf("�����Ĺ�����Ϣ��\n");
	        for (i = 0; i < all_monsters_size; i++) {
	            printf("����%s[%d]���ȹ�Ϊ:%d��Ѫ��Ϊ:%d\n", all_monsters[i].name, i + 1, all_monsters[i].initiative, all_monsters[i].health);
	        }
	       
	        // �ͷŶ�̬������ڴ�
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
    		printf("�������������\n");
    		scanf("%d", &num);
    		// ������뻺��������ֹ�û�������������������뻻�з���Ӱ�����fgets
    		void clear_input_buffer();

    		Character *characters = (Character *)malloc(num * sizeof(Character));
    		for (i = 0; i < num; i++) {
        		characters[i].names = (char *)malloc(MAX_LENGTH * sizeof(char));
    		}

    		// ��ʾ����������ҵ����ƺ���������ֵ�ĸ�ʽҪ��
    		printf("�밴˳����������������ҵ����ƺ���������ֵ����ֵ֮���ÿո��������ȫ��������������ٰ��س����������룺\n");

    		// һ���Ի�ȡ������ҵ�����
    		fgets(input, sizeof(input), stdin);

    		// ��ʼ���������ں�������
    		int inputIndex = 0;

    		// ����������ҵ�����
    		for (i = 0; i < num; i++) {
        		// ȷ������δ������Χ�ҵ�ǰ�ַ����ǻ��з�
        		while (inputIndex < strlen(input) && input[inputIndex]!= '\n') {
            		sscanf(&input[inputIndex], "%s %d", characters[i].names, &characters[i].dexterities);

            		// �������������������ѽ����Ĳ���
            		inputIndex += strlen(characters[i].names) + strlen(" ") + strlen("%d") + 1;

            		// �������ֵ
            		characters[i].dexterity_modifiers = ex_modifiers(characters[i].dexterities);

            		// �����ȹ�ֵ
            		characters[i].initiatives = roll_initiative(characters[i].dexterity_modifiers);
				
					// ð��������ҽṹ�����鰴���ȹ�ֵ��С��������
					for (k = 0; k < num - 1; k++) {
						for (j = 0; j < num - k - 1; j++) {
    						if (characters[j].initiatives > characters[j + 1].initiatives) {
        						// �����ṹ��
        						Character temp = characters[j];
        						characters[j] = characters[j + 1];
        						characters[j + 1] = temp;
    						}
						}
					}
        		}
		
				// ��������Ϣ
				for(i=0; i < num; i++){
        			printf("��� %s ���ȹ���ֵΪ%d\n", characters[i].names, characters[i].dexterity_modifiers);
        			printf("%s���ȹ�Ϊ:%d\n", characters[i].names, characters[i].initiatives);
				}
        		// �������з���׼��������һ����ҵ�����
				if (inputIndex < strlen(input)) {
    				inputIndex++;
				}
			}		

    		// �ͷŶ�̬������ڴ�
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

//�������ֵ�����������ڸ������������ֵ���㲻����ɵĴ��� 
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
        // ����ÿ�����ӵ������������Χ��1����������
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
