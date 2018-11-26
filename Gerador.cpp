##################################################
# Kewin Lima                                     #
#                                                #
# Este programa gera um arquivo pseudo-código    #
# gcode para auxiliar no projeto CNCells         #
#                                                #
# 26/11/18                                       #
################################################## 
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	float Mx=0 , My =0;
	printf("(");
	for(My= 0; My < 1.1 ; My = My + 0.1)
	{
		for(Mx=0; Mx < 0.5 ; Mx = Mx + 0.1)
		{
			printf("\"G1 X%.2f Y%.2f\", \"G4 P150\", ", Mx, My );
		}
		printf("\n");
	}
	printf(")");
	
	system("pause");
	return 0;
}
