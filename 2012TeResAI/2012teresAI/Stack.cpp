#include"Data.h"

void push(stack *s, Action dir){
	if(s->count < 100){
		s->a[s->count++]=dir;
	}
}
Action pop(stack *s){
	if(s->count > 0){
		return s->a[--s->count];
	}else{
		s->count = 0;
		return STOP;
	}
}