#pragma once
#include "BF2Stl.h"

namespace bs
{
	// forward declare for storing a pointer to this in ConsoleObject class without nasty casting
	class string_functor;
}

/*
* Hide the gory details here
*/
namespace BF2
{
	namespace detail
	{
		template<class T>
		class CInfo
		{
		public:
			T object;
			unsigned int ID0;
			unsigned int ID1;
			unsigned int ID2;
		};

		class CClassManager
		{
		public:
			virtual void						Function0();
			virtual void						Function1();
			virtual void						Function2();
			virtual void						Function3();
			virtual void						Function4();
			virtual void						Function5();
			virtual void						Function6();
			virtual void						Function7();
			virtual void						Function8();
			virtual void						Function9();
			virtual void						Function10();
			virtual void						Function11();
			virtual void						Function12();
			virtual void						Function13();
			virtual void						Function14();
			virtual void						Function15();
			virtual void						Function16();
			virtual void						Function17();
			virtual void*						getClassByName(const std::string* name);

			char								unknown4[44];		//0x0004
			std::map<std::string, CInfo<void*>> classMap;			//0x0030  
			char								unknown5[4];		//0x003C
			std::map<std::string, void*>		internalClassMap;	//0x0040  
		};

		class CMainConsole
		{
		public:
			virtual void	Function0();
			virtual void	Function1();
			virtual void	Exec(std::string* cmd, std::string* retVal, int i1 = 0, int i2 = 0, int i3 = 0, int i4 = 0, int i5 = 1);
			virtual void	Function3();
			virtual void	Print(std::string* strText);
			char			unknown0[292]; // 0x004
			char*			currentConFile; // 0x128
											//char			unknown1[xx]; // 0x12C
											//int				currentLine; // 0x2CC ignoring empty lines

		};

		class CMainConsoleObject;

		class CMainConsoleObjects
		{
		public:
			void*													unk;
			char													unknown0[8];
			std::map<std::string, CMainConsoleObject*>				entryList;
			char													unknown1[48];
		};

		class CMainConsoleObject
		{
		public:
			virtual void function0();
			virtual void function1();
			virtual void function2();
			virtual void function3();
			virtual void function4();
			virtual void function5();
			virtual void function6();
			virtual void function7();
			virtual void function8();
			virtual void function9();
			virtual void function10();
			virtual void function11();
			virtual void function12();
			virtual void function13();
			virtual void function14();
			virtual void function15();
			virtual void function16();
			virtual void function17();
			virtual void function18();
			virtual void function19();
			virtual void function20();
			virtual void invoke(std::string* result, std::string* args, int numArgs);
			virtual void function22();
			virtual void invoke();
			virtual void function24();
			virtual void function25();
			virtual void function26();
			virtual void function27();
			virtual void function28();
			virtual void function29();
			virtual void function30();
			virtual void function31();
			virtual void function32();

			int isGetSet;
			const char* nodeName; //0x0008  
			union //0x000C
			{
				const char* name;
				bs::string_functor* pRFXObject;
			};
			int authorizationLevel; // 0x0010
									//char unknown16[8]; // 0x0014
			int i0;
			int minNumArgs;
			int maxNumArgs; //0x001C
			int i2;
			const char* arg1; //0x0024  
			const char* arg2; //0x0028  
			const char* arg3; //0x002C  
			char unknown48[28]; //0x0030
			const char* returnType; //0x004C  
			const char* argType1; //0x0050  
			const char* argType2; //0x0054  
			const char* argType3; //0x0058  
			char unknown92[76]; //0x005C 
		};//Size=0x00A8(168)

		class CSwiffHost
		{
		public:
			virtual void function0();	// 0x00
			virtual void function1();	// 0x04
			virtual void function2();	// 0x08
			virtual void function3();	// 0x0C
			virtual void function4();	// 0x10
			virtual void Load();	// 0x14
			char unknown0[3700]; // 0x04
			int mouseAreaWidth; // 0xE78
			int mouseAreaHeight; // 0xE7C
		};


		class CSwiffPlayer
		{
		public:
			virtual void function0();	// 0x00
			virtual void function1();	// 0x04
			virtual void function2();	// 0x08
			virtual void function3();	// 0x0C
			virtual void function4();	// 0x10
			virtual void function5();	// 0x14
			virtual void function6();	// 0x18
			virtual void function7();	// 0x1C
			virtual void function8();	// 0x20
			virtual void function9();	// 0x24
			virtual void function10();	// 0x28
			virtual void function11();	// 0x2C
			virtual void function12();	// 0x30
			virtual void function13();	// 0x34
			virtual void function14();	// 0x38
			virtual void function15();	// 0x3C
			virtual void function16();	// 0x40
			virtual void function17();	// 0x44
			virtual void function18();	// 0x48
			virtual void function19();	// 0x4C
			virtual void function20();	// 0x50
			virtual void function21();	// 0x54
			virtual void function22();	// 0x58
			virtual void function23();	// 0x5C
			virtual void function24();	// 0x60
			virtual void function25();	// 0x64
			virtual void function26();	// 0x68
			virtual void function27();	// 0x6C
			virtual void function28();	// 0x70
			virtual void function29();	// 0x74
			virtual void function30();	// 0x78
			virtual void function31();	// 0x7C
			virtual void function32();	// 0x80
			virtual void updateMouse(int, int, int);	// 0x84
			virtual void function34();	// 0x88
			virtual void function35();	// 0x8C
			virtual void function36();	// 0x80
		};

		class CObject;
		class CSoldier;

		class CPlayer
		{
		public:
			virtual void				Function0();
			virtual void				Function1();
			virtual void				Function2();
			virtual void				Function3();
			virtual void				Function4();
			virtual void				Function5();
			virtual void				Function6();
			virtual void				Function7();
			virtual std::string*		getName();
			virtual void				Function9();
			virtual void				Function10();
			virtual void				Function11();
			virtual void				Function12();
			virtual void				Function13();
			virtual void				Function14();
			virtual void				Function15();
			virtual void				Function16();
			virtual void				Function17();
			virtual void				Function18();
			virtual void				Function19();
			virtual void				Function20();
			virtual void				Function21();
			virtual void				Function22();
			virtual void				Function23();
			virtual bool				getIsAI();
			virtual void				Function25();
			virtual bool				GetIsAlive();
			virtual void				Function27();
			virtual void				kill();
			virtual bool				Connected();
			virtual void				Function30();
			virtual void				Function31();
			virtual void				Function32();
			virtual void				Function33();
			virtual void				Function34();
			virtual void				Function35();
			virtual void				Function36();
			virtual void				Function37();
			virtual void				Function38();
			virtual void				Function39();
			virtual CObject*			getSoldierObject();
			virtual void				Function40();
			virtual void				Function41();
			virtual void				Function42();
			virtual void				Function43();
			virtual void				Function44();
			virtual void				Function45();
			virtual void				Function46();
			virtual void				Function47();
			virtual void				Function48();
			virtual void				Function49();
			virtual void				Function50();
			virtual void				Function51();
			virtual void				Function52();
			virtual void				Function53();
			virtual void				Function54();
			virtual void				Function55();
			virtual void				Function56();
			virtual void				Function57();
			virtual void				Function58();
			virtual void				Function59();
			virtual void				Function60();
			virtual void				Function61();
			virtual void				Function62();
			virtual void				Function63();
			virtual void				Function64();
			virtual void				Function65();
			virtual void				Function66();
			virtual void				Function67();
			virtual void				Function68();
			virtual void				Function69();
			virtual void				Function70();
			virtual void				Function71();
			virtual void				Function72();
			virtual void				Function73();
			virtual void				Function74();
			virtual void				Function75();
			virtual void				Function76();
			virtual void				Function77();
			virtual void				Function78();
			virtual void				Function79();
			virtual void				Function80();
			virtual void				Function81();
			virtual void				Function82();
			virtual void				Function83();
			virtual void				Function84();
			virtual void				Function85();
			virtual void				Function86();
			virtual void				Function87();
			virtual void				Function88();
			virtual void				Function89();
			virtual void				Function90();
			virtual void				Function91();
			virtual void				Function92();
			virtual void				Function93();
			virtual void				Function94();
			virtual void				Function95();
			virtual void				Function96();
			virtual void				Function97();
			virtual void				Function98();
			virtual void				Function99();
			virtual void				Function100();
			virtual void				Function101();
			virtual void				Function102();
			virtual void				Function103();
			virtual void				Function104();
			virtual void				Function105();
			virtual void				Function106();
			virtual void				Function107();
			virtual void				Function108();
			virtual void				Function109();
			virtual void				Function110();
			virtual void				Function111();
			virtual void				Function112();
			virtual void				Function113();
			virtual void				Function114();
			virtual void				Function115();
			virtual void				Function116();
			virtual void				Function117();
			virtual void				Function118();
			virtual void				Function119();
			virtual void				Function120();
			virtual void				Function121();
			virtual void				Function122();
			virtual void				Function123();
			virtual void				Function124();
			virtual void				Function125();
			virtual void				Function126();
			virtual void				Function127();
			virtual void				Function128();
			virtual void				Function129();
			virtual void				Function130();
			virtual void				Function131();
			virtual void				Function132();
			virtual void				Function133();
			virtual void				Function134();
			virtual void				Function135();
			virtual void				Function136();
			virtual void				Function137();
			virtual void				Function138();
			virtual void				Function139();
			virtual void				Function140();
			virtual void				Function141();
			virtual void				Function142();
			virtual void				Function143();
			virtual void				Function144();
			virtual void				Function145();
			virtual void				Function146();
			virtual void				Function147();
			virtual int					getUnlockLevel(int maxLevel, int kit, int nextLevel);
			virtual void				Function149();

			char						unknown0[84];		// 0x0004
			int							kitID;				// 0x0058
			char						unknown1[4];		// 0x005C
			std::string					playerName;			// 0x0060
			char						unknown2[4];		// 0x007C
			void*						pVehicle;			//0x0080  
			void*						pCamera;			//0x0084  
			char						unknown136[44];		//0x0088
			int							vehicleSeatIndex;	//0x00B4
			int							playerID;			//0x00B8
			char						unknown3[4];		//0x00BC
			float						FieldOfView;		//0x00C0
			float						Zoom1;				//0x00C4
			float						Zoom2;				//0x00C8
			CSoldier*					pSoldier;			//0x00CC
			char						unknown4[8];		//0x00D0
			int							team;				//0x00D8
			char						unknown5[12];		//0x00DC
			bool						playerDown;			//0x00E8
			char						unknown6[15];		//0x00EC
			unsigned int				ping;				//0x00F8
			char						unknown7[16];		//0x00FC
			int							squadNumber;		//0x010C 
			bool						isCommander;		//0x0110 
			bool						isSquadLeader;		//0x0111 
			char						unknown8[18];		//0x0112					
			bool						isInsideCP;			//0x0124 
			char						unknown9[95];		//0x0128
			bool						primaryFireDown;	//0x0184
			bool						secondaryFireDown;	//0x0185
			char						unknown10[1];		//0x0186
			bool						sprinting;			//0x0187 
			bool						sprintingCur;		//0x0188 
			char						unknown11[175];		//0x0189
			int							weaponFireNum;		//0x0238
			int							curWeaponAmmo;		//0x023C
			char						unknown12[8];		//0x0240
			int							curWeaponMaxAmmo;	//0x0248
			char						unknown13[8];		//0x024C
			int							curWeaponFireMode;	//0x0250
			char						unknown14[8];		//0x0254
			int							curWeaponZoomed;	//0x025C
			char						unknown15[92];		//0x0260
			std::map<int, int>			unlocks[2];
		};

		class CPlayerManager
		{
		public:
			virtual void		Function0();
			virtual void		Function1();
			virtual void		Function2();
			virtual void		Function3();
			virtual void		Function4();
			virtual void		Function5();
			virtual void		Function6();
			virtual void		Function7();
			virtual CPlayer*	getPlayerByIndex(int Index);
			virtual void		Function8();
			virtual void		Function9();
			virtual void		Function10();
			virtual CPlayer*	getLocalPlayer_();
		};

		class CHudInformationLayer
		{
		private:
			virtual void Function0(); //0
			virtual void Function1(); //4
			virtual void Function2(); //8
			virtual void Function3(); //C
			virtual void createBool(std::string* name, bool* pVar); //10
			virtual bool* getBoolByIndex(int index); //14
			virtual bool* getBoolByName(std::string* name); //18
			virtual void createInt(std::string* name, int* pVar); //1C
			virtual int* getIntByIndex(int index); //20
			virtual int* getIntByName(std::string* name); //20
			virtual void createFloat(std::string* name, float* pVar); //28
			virtual float* getFloatByIndex(int index); //2C
			virtual float* getFloatByName(std::string* name); //30
			virtual void createString(std::string* name, char** pVar); //34
			virtual char** getStringByIndex(int index); //38
			virtual char** getStringByName(std::string* name); //3C
		};
	}
}