#include <stdio.h>
#include <string.h>
#define N 10
int keynum=5; //키워드 개수
typedef struct w
{
    char word[16];
    int repeat=0; //반복된 횟수
    int links=0; //연관된 단어의 개수
    w* link[999]; //연관된 단어
    double first=0; //나눠줄 가중치
    double last=0; //받을 가중치
};
w wordlist[1000];
w* wordarray[1000]; //단어 순서대로
w* wlpoint[1000];
int wlindex=0, wllength=0;
w* sorted[10];
double o[1000];
w* op[1000];
int except(char input[]) //리스트에서 제외할 단어
{
    FILE *fp;
    char ex[16];
    int k=1;
    fp=fopen("excepts.txt", "r");
    while(feof(fp)==0)
    {
        fscanf(fp,"%s",ex);
        if(!strcmp(ex,input))
        {
            k=0;
            break;
        }
    }
    if(k==1) return 1;
    return 0;
}
int exist(char input[]) //wordlist에 word가 존재하는지
{
    int i;
    if(wllength==0) return -1;
    for(i=0; i<wllength; i++)
    {
        if(!strcmp(wordlist[i].word,input))
        {
            break;
        }
    }
    if(i==wllength) return -1;
    else return i;
}
int vowel(char a) //모음이면 1
{
    return a=='a'||a=='i'||a=='e'||a=='u'||a=='o';
}
char* plural(char input[]) //복수형
{
    int len=strlen(input);
    char guess[16];
    strcpy(guess,input);
    if(input[len-2]=='e'&&input[len-1]=='s')  //wordlist에 단수형 단어가 이미 저장되어있을 경우
    {
        if(input[len-3]=='i'&&(!vowel(input[len-4]))) //-자음+ies에 대해 ex. baby->babies
        {
            guess[len-2]='\0';
            guess[len-3]='y';
        }
        else if(input[len-3]=='x'||input[len-3]=='s'||(input[len-3]=='h'&&(input[len-4]=='c'||input[len-4]=='s')))
        {
            guess[len-2]='\0';
        }
    }
    else if(input[len]=='s')
    {
        guess[len-1]='\0';
    }
    if(exist(guess)!=-1)
    {
        return guess;
    }
    strcpy(guess,input); //wordlist에 복수형 단어가 이미 저장되어있을 경우
    if(input[len-1]=='x'||input[len-1]=='s'||(input[len-1]=='h'&&(input[len-2]=='c'||input[len-2]=='s')))
    {
        guess[len]='e';
        guess[len+1]='s';
        guess[len+2]='\0';
    }
    else if(input[len-1]=='y'&&(!vowel(input[len-2])))
    {
        guess[len-1]='i';
        guess[len]='e';
        guess[len+1]='s';
        guess[len+2]='\0';
    }
    else
    {
        guess[len]='s';
        guess[len+1]='\0';
    }
    if(exist(guess)!=-1)
    {
        int where=exist(guess);
        strcpy(wordlist[where].word,input);
        return input;
    }
    return input;
}
void divwords() //wordlist에 넣기
{
    char name[20];
    char input[16];
    int i,index=0;
    FILE * fp;
    FILE *ftag;
    printf("파일 이름을 입력하세요.\n");
    do
    {
        scanf("%s",name);
        fp = fopen(name, "r");
        if(fopen(name,"r")==NULL) printf("File does not exists.");
    }
    while(fopen(name,"r")==NULL);
    fclose(fp);
    ftag=fopen("tags.txt","a");
    fputc('\n',ftag);
    fprintf(ftag,"%s ",name);
    fclose(ftag);
    fp=fopen(name,"r");
    while(feof(fp)==0)
    {
        fscanf(fp,"%s",input);
        if(input[strlen(input)-1]=='.'||input[strlen(input)-1]==',') input[strlen(input)-1]='\0';
        if(input[0]>='A'&&input[0]<='Z')
        {
            input[0]=input[0]+32;
        }
        if(except(input))
        {
            strcpy(input,plural(input));
            if(exist(input)==-1)
            {
                strcpy(wordlist[wllength].word,input);
                wordarray[wlindex]=&wordlist[wllength];
                wllength++;
            }
            else
            {
                int where=exist(input);
                wordlist[where].repeat++;
                wordarray[wlindex]=&wordlist[where];
            }
            int x=(wlindex-N)>0?(wlindex-5):0;
            int j;
            for(j=wlindex-1; j>=x; j--)
            {
                if(wordarray[j]!=NULL)
                {
                    wordarray[wlindex]->link[wordarray[wlindex]->links]=wordarray[j];
                    wordarray[wlindex]->links++;
                    wordarray[j]->link[wordarray[j]->links]=wordarray[wlindex];
                    wordarray[j]->links++;
                } //서로 링크해주고 링크개수++
            }
        }
        wlindex++;
    }
    for(i=0; i<wllength; i++)
    {
        wordlist[i].last=(double)1/(double)wllength;
    }

    int x,y,z;
    for(int y=0; y<10; y++)
    {
        for(x=0; x<wllength; x++)
        {
            wordlist[x].first=wordlist[x].last;
            wordlist[x].last=0;
        }
        for(x=0; x<wllength; x++)
        {
            for(z=0; z<wordlist[x].links; z++)
            {
                wordlist[x].last+=(wordlist[x].link[z]->first)/(wordlist[x].link[z]->links);
            }
        }
        for(x=0; x<wllength; x++)
        {
            o[x]=wordlist[x].last;
            op[x]=&wordlist[x];
        }
    }
}
void sorting()
{
    int i,j;
    double temp1;
    w* temp2;
    for(i=0; i<wllength; i++)
    {
        for(j=i+1; j<wllength; j++)
        {
            if(o[i]<o[j])
            {
                temp1=o[i];
                o[i]=o[j];
                o[j]=temp1;
                temp2=op[i];
                op[i]=op[j];
                op[j]=temp2;
            }
        }
    }

}
void mode1()
{
    printf("Test");
}
void mode2()
{

}

void scene1()
{
    int n;
    printf("------------------\n");
    printf("1.신규 파일 저장\n");
    printf("2.키워드 추출\n");
    printf("4. 종료\n");
    printf("모드를 선택하세요.\n");
     printf("------------------\n");
    scanf("%d",&n);
    switch(n)
    {
    case 1:
        mode1();
        break;
    case 2:
        mode2();
        break;
    case 4:
        return ;
    }
}

int main()
{
    scene1();
}
