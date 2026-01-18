#include <stdio.h>

#define ll long long

int main(){
	ll x, y, z;
	while(true) {
		scanf("%lld %lld %lld", &x, &y, &z);
		
		if(x == 0 && y == 0 && z == 0) break;
		ll ap1 = y-x, ap2 = z-y;
		ll gp1 = y/x, gp2 = z/y;
		
			
		//printf("%lld %lld\n", gp1, gp2);
		
		if(ap1 == ap2){
			printf("AP %lld\n", z+ap1);
		}
		else if(gp1 == gp2){
			printf("GP %lld\n", z*gp1);
		}
	}
}

