// 1.
// Add set of random indicator values to be evaluated as features. 
// Add random-guess classifier. see http://www.r-bloggers.com/computing-classification-evaluation-metrics-in-r/ 
// 2.
// Add variable interactions
// IF( X = Y, X(.1), LOG( ( 201) - X ) / (  (201) - Y), (201 - Y) ) ) <--round result to the 9th decimal place
// =IF(F3=G3,F3*0.1,ROUND(LOG((201-F3)/(201-G3),(201-G3)),9))

Var: {int RightStrength(0),}int rsLimit(0);
//RightStrength = 2;
rsLimit = 6;
//While RightStrength <= rsLimit begin


vars:
	leftStengthCounter(0)
	,rightStengthCounter(0)
	,leftPatternCounter(0)
	,rightPatternCounter(0)
	,bool calcSwingHi(True)
	,bool calcSwingLo(False)
	,plotBarNumTxt(0)
	,plotSwingHiTxt(0)
	,plotSwingLoTxt(0)
	,int BarsAgoSHiBar(0)
	,int BarsAgoSLoBar(0)
	,int SwingHiAtBarNum(0)
	,int SwingLoAtBarNum(0)
	,int lastSwingHiBarNum(0)
	,int lastSwingLoBarNum(0)
	,counterSH(0)
	,counterSL(0)
	,sizeOfLastSwing(0)
	,sHiBarNumForCalc(0)
   	,sLoBarNumForCalc(0)   	
   	,sHiPriceForCalc(0) 
   	,sLoPriceForCalc(0)
	,state("") 	
	//,bool debug(false) 
	,bool debugToCommentLog(True)
	,bool debugToPrintLog(True) 
	,bool debugToCSV(True)
	,int swingBarHiCount(0)
	,int swingBarLoCount(0);

//BarsAgoSHiBar = 0;
//BarsAgoSLoBar = 0;
//SwingHiAtBarNum = 0;
//SwingLoAtBarNum = 0;
swingBarHiCount = 0;
swingBarLoCount = 0;

array: swingHi[50,3](0)
	,swingLo[50,3](0);
	//Array: MyNumericArray[10,5] (0);

	//Var: BarCounter(0);	

	//if High > Highest(High,10)[1] and BarCounter < 10 then begin
	//	MyNumericArray[BarCounter,1] = High;
	//	MyNumericArray[BarCounter,2] = Low;
	//	MyNumericArray[BarCounter,3] = Open;
	//	MyNumericArray[BarCounter,4] = Close;
	//	MyNumericArray[BarCounter,5] = BarNumber;
	//	BarCounter = BarCounter + 1;
	//end;

leftStengthCounter = 1;  //LeftStrength;
rightStengthCounter = 1; //RightStrength;
leftPatternCounter = 0;
rightPatternCounter = 0;

inputs: 
	bool debug(True)
	,int LeftStrength(8)   //Sets the required number of bars on the left side of the pivot bar.
	,int RightStrength(12) //Sets the required number of bars on the right side of the pivot bar.
	,int leftPad(0)
	,int rightPad(0)
	,SwingHiObject(High)
	,SwingLoObject(Low)
	,bool ShowSwings(True)
	,bool ShowSwingBarNum(True)
	,bool ShowObjVal(False)
	,bool ShowObjDiff(False)
	,bool ShowBarNumbers(False)
	,bool ReadWriteToDB(False)
	,bool UseBucketData(False)
	,int loadGroupId(1004);
	
{**********************************
 ** meta data
------ ------------------------
Pos. 1 barNumber
Pos. 2 symbol 
Pos. 3 lengthOfSeries
Pos. 4 chartType
Pos. 5 chartInterval
Pos. 6 dateBarOpened
Pos. 7 dayOfWeekBarOpened
Pos. 8 timeBarOpened
Pos. 9 timeBarOpenedhhmmss
Pos. 10 minutesBarWasOpen
Pos. 11 barOpenPrice
Pos. 12 barHighPrice
Pos. 13 barLowPrice
Pos. 14 barClosePrice
Pos. 15 minUnitPriceChange
Pos. 16 settingMaxBarsBack
**********************************}
//begin meta data
vars: 
	 int seriesBarNumber(0)
	,string dataSeriesSymbol("") //symbolName
	,int lengthOfSeries(0)
	,int chartType(0) //BarType where 0 = TickBar, 1 = Minute, 2 = Daily, 3 = Weekly, 4 = Monthly, 5 = Point & Figure, 6 = (reserved), 7 = (reserved), 8 = Kagi, 9 = Kase, 10 = Line Break, 11 = Momentum, 12 = Range, 13 = Renko, 14 = Second
	,chartInterval(0) 
	,string dateBarOpened("")
	,string dayOfWeekBarOpened("")
	,string dateTimeBarOpened("") 
	,string timeBarOpenedhhmmss("")
	,minutesBarWasOpen(0)
	,double barOpenPrice(0)
	,double barHighPrice(0)
	,double barLowPrice(0)
	,double barClosePrice(0)
	,double minUnitPriceChange(0) //PriceScale = BigPointValue/PointValue 
 	,int settingMaxBarsBack(0)

	,string strtimeBarOpened("")
	,string strHHBarOpened("")
	,string strMMBarOpened("")
	,string strSSBarOpened("")
	,string strDateBarOpened("")
	,string strDateYYBarOpened("")
	,string strDateMMBarOpened("")
	,string strDateDDBarOpened("")
	,string strDateDowBarOpened("")
	,julianDateTime(0)
	,numMinutesFromMidnit(0)
	,numminutesBarWasOpn(0);


//Pos. 1 barNumber
//Returns the number of the current bar. 
//Each bar, subsequent to the initial number of bars specified by the Maximum Bars Back setting, is assigned a sequential number; the initial bars specified by the setting are not numbered. 
//For example, if Maximum Bars Back is set to 20, the 21st bar will be assigned a number of 1. 
seriesBarNumber = CurrentBar + settingMaxBarsBack; //or CurrentBar + MaxBarsBack;

//Pos. 2 symbol
//Returns a string expression containing the name of the symbol that the study is applied to
dataSeriesSymbol = SymbolName ;
 
//Pos. 3 lengthOfSeries
//Returns a numerical value indicating the actual number of bars of a data series on chart
//If the series is not updating in real-time (no new bars appear), the keyword returns the same value on each calculation. If the chart updates in real-time the keyword returns the number of bars available by the moment script references the keyword. It means that the keyword possibly can return different values even during calculation within the same bar. 
lengthOfSeries = seriesBarNumber ;//Symbol_Length ; 
 
//Pos. 4 chartType
//Returns a numerical value, indicating the resolution units of the data series that the study is applied to. 
chartType = BarType ;
{***********************************************************************************************************
	values returned for each type of resolution units: 
	  0 = Ticks (Ticks & Contracts)			8 = Kagi
	  1 = Intra-Day (Seconds, Minutes, & Hours)	9 = (Reserved for future use)
	  2 = Days						10 = Line Break
	  3 = Weeks						11 = (Reserved for future use)
	  4 = Months, Quarters, & Years			12 = (Reserved for future use)
	  5 = Points, Changes, Point & Figure		13 = Renko
	  6 = (Reserved for future use)			256 = Heikin Ashi
	  7 = (Reserved for future use)	  
***********************************************************************************************************}

//Pos. 5 chartInterval
//Returns a numerical value, indicating the number of resolution units (bar interval) of the data series that the study is applied to. 
//Returns the number of Ticks, Contracts, Points, Changes, Seconds, Minutes, Hours, Days, Weeks, Months, Quarters, or Years, depending on the chart resolution; a value of 5 will be returned for a 5-second as well as for a 5-tick chart. 
chartInterval = High[RightStrength] - Low[RightStrength] ; //If range bars in TradeStation use "BoxSize". If range bars in MultiCharts use "BarType"  

//Pos. 6 dateBarOpened 
strDateBarOpened = (FormatDate( "yyyy-MM-dd", ElDateToDateTime( Date[RightStrength] ) ));
dateBarOpened = strDateBarOpened ;

//Pos. 7 dayOfWeekBarOpened
dayOfWeekBarOpened = (FormatDate( "ddd", ElDateToDateTime(Date[RightStrength])));

//Pos. 8 timeBarOpened
//TimeToMinutes function calculates the number of minutes from midnight for any 24-hour (HHMM) time value
julianDateTime = DateToJulian(Date[RightStrength])+(TimeToMinutes(Time[RightStrength]) / 60 / 24 ) ; 
//timeBarOpened =(FormatTime("HH:mm:ss tt",julianDateTime));
//dateTimeBarOpened =DateTimeToString_Ms(DateTime[RightStrength]);
dateTimeBarOpened = DateTimeToString(julianDateTime );
  //DateTimeToString_Ms(DateTime[0])  will return a string value corresponding to the time and time of the current bar "5/28/2013 08:41:11.871". 

//Pos. 9 timeBarOpenedhhmmss
strHHBarOpened = FormatTime("HH",julianDateTime);
strMMBarOpened = FormatTime("mm",julianDateTime);
strSSBarOpened = FormatTime("ss",julianDateTime);
timeBarOpenedhhmmss = strHHBarOpened+":"+strMMBarOpened+":"+strSSBarOpened; //hh:mm:ss
//timeBarOpenedhhmmss = FormatTime("HH:mm:ss", ELTimetodatetime_s(Time_s[RightStrength]));

//Pos. 10 numminutesBarWasOpn 
//Time returns the time in 24-hour numeric format (HHMM format) of the closing price of the current bar
numMinutesFromMidnit = TimeToMinutes( Time[RightStrength] ); 
//numMinutesFromMidnit = SecondsFromDateTime(DateTime[RightStrength]) ;
If Date[RightStrength] = Date[RightStrength+1] Then Begin //date of current bar is the same as the date of one bar ago
	numminutesBarWasOpn = (numMinutesFromMidnit-1440) - (numMinutesFromMidnit[1]-1440);
End
Else Begin
 	numminutesBarWasOpn = (numMinutesFromMidnit+1440) - numMinutesFromMidnit[1]; 
End;

//Pos. 11 barOpenPrice
barOpenPrice = Open[RightStrength];

//Pos. 12 barHighPrice
barHighPrice = High[RightStrength];

//Pos. 13 barLowPrice
barLowPrice = Low[RightStrength];

//Pos. 14 barClosePrice
barClosePrice = Close[RightStrength];

//Pos. 15 minUnitPriceChange
minUnitPriceChange = minmove / pricescale; //PriceScale = BigPointValue / PointValue 
  {**************************************************************************************************
  ** MinMove / PriceScale calculates the minimum price move for a share or contract.
  ** MinMove / PriceScale * BigPointValue returns the profit or loss value associated with a minimum price move of a share or contract.
  ***************************************************************************************************}
  
//Pos. 16 settingMaxBarsBack
//Returns a numerical value indicating the Maximum Bars Back setting for the study. 
//All studies based on past data use a certain number of bars for their calculations. The number of bars is called Maximum number of bars a study will reference, or Maximum Bars Back. 
settingMaxBarsBack = MaxBarsBack + 1 ;

condition1 = seriesBarNumber > settingMaxBarsBack ;
	
	//plot bar numbers
	if ShowBarNumbers then begin
		plotBarNumTxt = text_new(Date[RightStrength]
				,Time[RightStrength]
				,SwingHiObject[RightStrength]+(minUnitPriceChange * 10)
				,numToStr(seriesBarNumber-RightStrength,0)
					);
		Text_SetStyle(plotBarNumTxt,2,1);
		text_setcolor(plotBarNumTxt,RGB(255,128,0)); 
	end;


var: 
	leftPadAmt(0),
	rightPadAmt(0);	

leftPadAmt = minUnitPriceChange * leftPad;			
rightPadAmt = minUnitPriceChange * rightPad;

var: bool leftHiPatternExist(FALSE),bool rightHiPatternExist(FALSE);
if condition1 and calcSwingHi then begin
	while leftStengthCounter <= LeftStrength AND leftPatternCounter <= LeftStrength begin
		//detect left pattern of SwingHi
		leftHiPatternExist = (SwingHiObject[RightStrength] >= SwingHiObject[RightStrength+leftStengthCounter]);

		if leftHiPatternExist then
				leftPatternCounter = leftPatternCounter + 1
			else
				leftPatternCounter = 0 ;
		
		leftStengthCounter = leftStengthCounter + 1 ;

		//detect right pattern of SwingHi
		while leftPatternCounter = LeftStrength and rightStengthCounter <= RightStrength  begin 
			rightHiPatternExist = (SwingHiObject[RightStrength] > SwingHiObject[RightStrength-rightStengthCounter])
									and (SwingHiObject[RightStrength] - Lowest(SwingLoObject,RightStrength)) >= rightPadAmt
									and (SwingHiObject[RightStrength] - Highest(SwingHiObject,RightStrength-1)) >= leftPadAmt;

			if rightHiPatternExist then
					rightPatternCounter = rightPatternCounter + 1
				else
					rightPatternCounter = 0 ;
			
			if rightPatternCounter = RightStrength then begin 

				SwingHiAtBarNum = seriesBarNumber-RightStrength;
				swingBarHiCount = seriesBarNumber-RightStrength-swingLo[0,0];	
				lastSwingHiBarNum = seriesBarNumber-RightStrength;
	
				for counterSH = 0 to 49 begin
					swingHi[50-counterSH,0] = swingHi[50-counterSH-1,0];
					swingHi[50-counterSH,1] = swingHi[50-counterSH-1,1];
					swingHi[50-counterSH,2] = swingHi[50-counterSH-1,2];
					swingHi[50-counterSH,3] = swingHi[50-counterSH-1,3];
				end; 
				swingHi[0,0] = lastSwingHiBarNum ; //barNum
				swingHi[0,1] = SwingHiObject[RightStrength] ; //price
				swingHi[0,2] = Date[RightStrength] ; //date
				swingHi[0,3] = Time[RightStrength] ; //time	
				
				if ShowSwings then begin
					plot1[RightStrength](SwingHiObject[RightStrength], "swingHiBar");
					plot2[RightStrength](SwingHiObject[RightStrength]+(minUnitPriceChange*60), "TopMargin");
				end;
				
				if swingHi[1,1] > 0 then begin
					if ShowObjDiff then begin
						Value1 = text_new(swingHi[1,2]//Date[RightStrength]
								,swingHi[1,3]//Time[RightStrength]
								,swingHi[1,1]+(minUnitPriceChange*17)//SwingHiObject[RightStrength]+(minUnitPriceChange*17)
								,"Size: +"+numToStr(swingLo[0,1]-swingHi[1,1],4)+" / "+numToStr((swingLo[0,0]-swingHi[1,0]),0)+" bars"
									);
						Text_SetStyle(Value1 ,2,1);
						text_setcolor(Value1 ,RGB(255,128,0)); 
					end;
					
					if ShowObjVal then begin
						Value2 = text_new(swingHi[1,2]//Date[RightStrength]
								,swingHi[1,3]//Time[RightStrength]
								,swingHi[1,1]+(minUnitPriceChange*9)//SwingHiObject[RightStrength]+(minUnitPriceChange*9) //Price
								,"Price: "+numToStr(swingHi[1,1],4)
									);
						Text_SetStyle(Value2 ,2,1);
						text_setcolor(Value2 ,RGB(255,128,0)); 
					end;
				end;
				
				if ShowSwingBarNum then begin
					plotSwingHiTxt = text_new(swingHi[1,2]//Date[RightStrength]
							,swingHi[1,3]//Time[RightStrength]
							,swingHi[1,1]+(minUnitPriceChange*1)//SwingHiObject[RightStrength]+(minUnitPriceChange*1)
							,"BarNum: "+numToStr(swingHi[1,0],0)
								);
					Text_SetStyle(plotSwingHiTxt,2,1);
					text_setcolor(plotSwingHiTxt,RGB(255,128,0)); 
				end;
			end; 

			rightStengthCounter = rightStengthCounter + 1 ;
		end;
		
		BarsAgoSHiBar = seriesBarNumber-SwingHiAtBarNum;
		rightStengthCounter = 1;
		rightPatternCounter = 0; 
		rightHiPatternExist = False;	
	end;
	
	leftStengthCounter = 1;
	leftPatternCounter = 0;
	leftHiPatternExist = False;
	calcSwingHi = False;
	calcSwingLo = True;

end;

var: bool leftLoPatternExist(FALSE),bool rightLoPatternExist(FALSE);
if condition1 and calcSwingLo then begin
	while leftStengthCounter <= LeftStrength AND leftPatternCounter <= LeftStrength begin
		//detect left pattern of SwingLo
		leftLoPatternExist = (Low[RightStrength] <= Low[RightStrength+leftStengthCounter]);

		if leftLoPatternExist then
				leftPatternCounter = leftPatternCounter + 1
			else
				leftPatternCounter = 0 ;
		
		leftStengthCounter = leftStengthCounter + 1 ;
		
		//detect right pattern of SwingLo
		while leftPatternCounter = LeftStrength and rightStengthCounter <= RightStrength  begin 
			rightLoPatternExist = (SwingLoObject[RightStrength] < SwingLoObject[RightStrength-rightStengthCounter])
									and (Highest(SwingHiObject,RightStrength) - SwingLoObject[RightStrength]) >= rightPadAmt
									and (Lowest(SwingLoObject,RightStrength-1) - SwingLoObject[RightStrength]) >= leftPadAmt;
 
			if rightLoPatternExist then
					rightPatternCounter = rightPatternCounter + 1
				else
					rightPatternCounter = 0 ;
			
			if rightPatternCounter = RightStrength then begin 

				SwingLoAtBarNum = seriesBarNumber-RightStrength;
				swingBarLoCount = (swingHi[0,0])-(swingLo[1,0]);
				lastSwingLoBarNum = seriesBarNumber-RightStrength;

				for counterSH = 0 to 49 begin
					swingLo[50-counterSH,0] = swingLo[50-counterSH-1,0];
					swingLo[50-counterSH,1] = swingLo[50-counterSH-1,1];
					swingLo[50-counterSH,2] = swingLo[50-counterSH-1,2];
					swingLo[50-counterSH,3] = swingLo[50-counterSH-1,3];
				end; 
				swingLo[0,0] = lastSwingLoBarNum ;  //barNum
				swingLo[0,1] = SwingLoObject[RightStrength] ; //price
				swingLo[0,2] = Date[RightStrength] ; //date
				swingLo[0,3] = Time[RightStrength] ; //time
				
				if ShowSwings then begin
					plot3[RightStrength](SwingLoObject[RightStrength], "swingLowBar");
					plot4[RightStrength](SwingLoObject[RightStrength]-(minUnitPriceChange*60), "LowMargin");
				end;
				
				
				if swingLo[1,1] > 0 then begin
					if ShowObjDiff then begin
						Value3 = text_new(swingLo[1,2]//Date[RightStrength]
								,swingLo[1,3]//Time[RightStrength]
								,swingLo[1,1]-(minUnitPriceChange*17)//SwingLoObject[RightStrength]-(minUnitPriceChange*17)
								,"Size: +"+numToStr(swingHi[0,1]-swingLo[1,1],4)+" / "+numToStr((swingHi[0,0]-swingLo[1,0]),0)+" bars"
							 	);
						Text_SetStyle(Value3 ,2,0);
						text_setcolor(Value3 ,RGB(255,128,0)); 
					end;				
	
					if ShowObjVal then begin
						Value4 = text_new(swingLo[1,2]//Date[RightStrength]
								,swingLo[1,3]//Time[RightStrength]
								,swingLo[1,1]-(minUnitPriceChange*9)//SwingLoObject[RightStrength]-(minUnitPriceChange*9)
								,"Price: "+numToStr(swingLo[1,1],4)
							 	);
						Text_SetStyle(Value4 ,2,0);
						text_setcolor(Value4 ,RGB(255,128,0)); 
					end;
				end;		

				if ShowSwingBarNum then begin
					plotSwingLoTxt = text_new(swingLo[1,2]//Date[RightStrength]
							,swingLo[1,3]//Time[RightStrength]
							,swingLo[1,1]-(minUnitPriceChange*1)//SwingLoObject[RightStrength]-(minUnitPriceChange*1)
							,"BarNum: "+numToStr(swingLo[1,0],0)
							 );
					text_SetStyle(plotSwingLoTxt,2,0);
					text_setcolor(plotSwingLoTxt,RGB(255,128,0));
				end;
			end; 

			rightStengthCounter = rightStengthCounter + 1 ;
		end; 
		
		BarsAgoSLoBar = seriesBarNumber-SwingLoAtBarNum;
		rightStengthCounter = 1;
		rightPatternCounter = 0;
		rightLoPatternExist = False; 	
	end;
	
	leftStengthCounter = 1;
	leftPatternCounter = 0;
	leftLoPatternExist = False;
	calcSwingHi = True;
	calcSwingLo = False;
end;


if CurrentBar + MaxBarsBack + 1 > MaxBarsBack + 1 then begin

		{*******************************************************
			swingLo[0,0] = lastSwingLoBarNum 	; //barNum
			swingLo[0,1] = Low[RightStrength] 	; //price
			swingLo[0,2] = Date[RightStrength] ; //date
			swingLo[0,3] = Time[RightStrength] ; //time
		*******************************************************}
		if condition1 then begin
			if swingHi[0,2] > swingLo[0,2] and //date
			swingHi[0,0] > swingLo[0,0] then begin //barNum
				//sizeOfLastSwing = AbsValue(swingHi[0,1] - swingLo[0,1]) ; //current swing high minus last swing low
				sHiBarNumForCalc =  swingHi[0,0] ;
				sLoBarNumForCalc = swingLo[0,0] ;
				sHiPriceForCalc =  swingHi[0,1] ;
				sLoPriceForCalc = swingLo[0,1] ;
			end;
			if swingLo[0,2] > swingHi[0,2] and //date
			swingLo[0,0] > swingHi[0,0] then begin //barNum
				//sizeOfLastSwing = AbsValue(swingHi[0,1] - swingLo[0,1]) ; //current swing high minus last swing low
				sHiBarNumForCalc =  swingHi[0,0] ;
				sLoBarNumForCalc = swingLo[0,0] ;
				sHiPriceForCalc =  swingHi[0,1] ;
				sLoPriceForCalc = swingLo[0,1] ;
			end;
		end;
			
		if condition1 then begin
			if //swingHi[0,2] > swingLo[0,2] and //date
			swingHi[0,0] > swingLo[0,0] and //barNum
			swingHi[0,1] > swingHi[1,1] and //price 
			swingHi[0,1] > swingLo[0,1] then begin //price
				state = "UP";
			end;
			if //swingLo[0,2] > swingHi[0,2] and //date
			swingLo[0,0] > swingHi[0,0] and //barNum
			swingLo[0,1] < swingLo[1,1] and //price 
			swingLo[0,1] < swingHi[0,1] then begin //price
				state = "DOWN";
			end;
		end;
		
		array: string stateUpDpwn[10,0]("");
		var: counterState(0);
		
		if condition1 then begin	
			for counterState = 0 to 9
			begin
				stateUpDpwn[10-counterState ,0] = stateUpDpwn[10-counterState -1,0];
				//_swingLo[1+counter] = _swingLo[0+counter];
			end; 
				stateUpDpwn[0,0] = state;
		end;	
		
				
		if debug then begin	
			Print("Bar:",numToStr(seriesBarNumber,0)
			//,"swingHiTime: ",FormatTime("HH:mm:ss", ELTimetodatetime_s(Time_s[RightStrength]))
			//," swingLoTime: ",FormatTime("HH:mm:ss", ELTimetodatetime_s(Time_s[RightStrength]))
			,", Last SwingHi [", BarsAgoSHiBar
			,"] bars ago @ bar [", SwingHiAtBarNum
			,"], Last SwingLo [", BarsAgoSLoBar
			,"] bars ago @ bar [",SwingLoAtBarNum
			//,"], sizeOfLastSwing [",sizeOfLastSwing
			,"]"
			,"state [",counterState
			,"]"
			+ NewLine);
		end;
	End;
	
{************************************************************************************************* 
**************************************************************************************************
zScore = ( x - Mean ) / SDev ;
		Mean = Average( x, Length ) ;
		SDev = StandardDev( x, Length, 2 ) ;

Normalize = (x - min[x]) / (max[x] - min[x]) //range set by min/max in sample

Denormalized = NormalizedValue * (max[x] - min[x]) + min[x]

linearlyRescale = (limit - floor) / (max[x] - min[x]) * (x - max[x]) + limit // where [x] is some lookback period and range is "hard coded" by setting [floor] and [limit] values to literals

rateOfChange = (x / x[Length] - 1 ) * 100
percentChange = (x - x[Length] ) / x[Length] ;

rsiNetChgAvg = (x - x[Length] ) / Length ;
************************************************************************************************** 
**************************************************************************************************}