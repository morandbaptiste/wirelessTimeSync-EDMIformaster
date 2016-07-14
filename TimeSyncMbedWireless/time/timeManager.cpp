/*
 * clock.c
 *
 * Created: 16/05/2016 17:44:39
 *  Author: MORAND Baptiste
 */ 
#include "timeManager.h" 




Clock timeManage={0,0,true};

bool sup(Clock clock1,Clock clock2){
		if(clock1.second>clock2.second){//clock1>clock2
			return true;
		}
		else{
			if(clock1.second<clock2.second){
				return false;
			}
			else{//egual
				if(clock1.halfmillis>clock2.halfmillis){
					return true;
				}
				else{
					return false;
				}
			}
		}

}
Clock sumClockPos(Clock clock1,Clock clock2 ){
	Clock ret={0,0,true};
	unsigned long int halfmillis=clock1.halfmillis;
	ret.second=clock1.second+clock2.second;
	halfmillis=halfmillis+clock2.halfmillis;
	ret.second=ret.second+(halfmillis/RTC_FREQ);
	ret.halfmillis=halfmillis%RTC_FREQ;
	return ret;
	
}
Clock sumClock(Clock clock1,Clock clock2 ){
	Clock ret={0,0,true};
		
	if((clock1.sign==true)&&(clock2.sign==true)){// + + +
		ret=sumClockPos(clock1,clock2);
	}
	else{
		if((clock1.sign==true)&&(clock2.sign==false)){ // + + -
			clock2.sign=true;
			ret=subClock(clock1,clock2);
		}
		else{
					if((clock1.sign==false)&&(clock2.sign==true)){ // - + + 
						clock1.sign=true;
						ret=subClock(clock2,clock1);
					}
					else{
						if((clock1.sign==false)&&(clock2.sign==false)){ // - + -
							clock1.sign=true;
							clock2.sign=true;
							ret=sumClockPos(clock1,clock2);
							ret.sign=false;
						}
					}
		}
		
	}
	ret.second+=ret.halfmillis/RTC_FREQ;
	ret.halfmillis=ret.halfmillis%RTC_FREQ;
	return ret;
}
Clock absClock(Clock clk){
	if(clk.sign==false){
		clk.sign=true;
	}
	return clk;
}
Clock subClock(Clock clock1,Clock clock2 ){
	Clock ret={0,0,true};
	if(clock1.sign!=clock2.sign){
		if(clock1.sign==true){// + - -
			clock2.sign=true;
			ret=sumClock(clock1,clock2);
		}
		else{// - - +
			clock1.sign=true;
			ret=sumClock(clock1,clock2);
			ret.sign=false;
		}
	}
	else{
		if(clock1.sign==true){ //+ - +
			if(sup(clock1,clock2)){//clock1>Clock2
				
				ret=subClockPos(clock1,clock2);
			}
			else{
				ret=subClockPos(clock2,clock1);
				ret.sign=false;
			}
		}
		else{// - - - 
			clock2.sign=true;
			clock1.sign=true;	
			ret=subClock(clock1,clock2);
			if(ret.sign==true){
				ret.sign=false;
			}
			else{
				ret.sign=true;
			}

				
		}
	}

	
	return ret;
}
Clock subClockPos(Clock clock1,Clock clock2 ){
	Clock ret={0,0,true};
	//printf("clock1:%ld,%ld\r\n",clock1.second,clock1.halfmillis);
	//printf("clock2:%ld,%ld\r\n",clock2.second,clock2.halfmillis);
	//clock1.second+=clock1.halfmillis/RTC_FREQ;
	//clock1.halfmillis=clock1.halfmillis%RTC_FREQ;	
		
	//clock2.second+=clock2.halfmillis/RTC_FREQ;
	//clock2.halfmillis=clock2.halfmillis%RTC_FREQ;
	long int halfmillis=clock2.halfmillis;
	ret.second=clock1.second-clock2.second;
	halfmillis=(long int)clock1.halfmillis-halfmillis;
	
	
	
	if(halfmillis<0){
		
		if(ret.second!=0){
			if (halfmillis<-RTC_FREQ)
			{
				/*pc.printf("halfMillis: %ld\r\n",halfmillis);
				pc.printf("c1:%ld,%ld",clock1.second,clock1.halfmillis);
				pc.printf("c2:%ld,%ld",clock2.second,clock2.halfmillis);*/
				//sendHMI("error clock");
			}
			
			ret.second--;
			ret.halfmillis=(unsigned int)(halfmillis+RTC_FREQ);
		}
	}
	else{
		ret.halfmillis=halfmillis;
	}
			
	return ret;
}