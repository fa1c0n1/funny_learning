/*

	题： 解密回文 —— 栈

 */

#include <stdio.h>
#include <string.h>

int main(void)
{
	char arr[50], stack[50];
	int len, mid, top = 0;

	printf("Please input backwritting: ");
	gets(arr); 

	len = strlen(arr);
	mid = len / 2 - 1; //求字符串的中点
	
	int i = 0;
	//将mid前的字符依次入栈
	for(; i <= mid; i++) {
		stack[++top] = arr[i];
	}

	int next;
	//判断字符串的长度是奇数还是偶数，并找出需要进行字符匹配的起始下标
	if(len % 2 == 0)
		next = mid + 1;
	else
		next = mid + 2;

	//开始匹配
	for(i = next; i < len; i++) {
		if(stack[top] != arr[i]) {
			break;
		}
		top--;
	}

	//如果top为0, 则说明栈内所有的字符都被一一匹配了.
	if(top == 0)
		printf("YES, it's backwritting!!!\n");
	else
		printf("NO, it isn't backwritting!!!\n");

	return 0;
}