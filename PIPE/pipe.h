#ifndef PIPE_H
#define PIPE_H
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
using namespace std;
#include "Const.h"
#include "LogicalUnits.h"
#include "HardwareUnits.h"
struct W_ { 
	int stall, begin;
	
	void LogicProc(int W_stat) {
		stall = set<int> { SADR, SINS, SHLT }.count(W_stat);
	}
	
	int stat, icode, valE, valM, dstE, dstM;

	void Print(char *buf) {
		sprintf(buf, "%s\"W\" : { ", buf);
		sprintf(buf, "%s\"begin:\" : 0x%x, ",buf , begin);
		sprintf(buf, "%s\"stall\" : %d, ",buf ,stall);
		sprintf(buf, "%s\"stat\" : %d, ",buf ,stat);
		sprintf(buf, "%s\"icode\" : %d, ",buf ,icode);
		sprintf(buf, "%s\"valE\" : %d, ",buf ,valE);
		sprintf(buf, "%s\"valM\" : %d, ",buf ,valM);
		sprintf(buf, "%s\"dstE\" : %d, ",buf ,dstE);
		sprintf(buf, "%s\"dstM\" : %d ",buf ,dstM);
		sprintf(buf, "%s}\n", buf);
	}
};

struct M_ { 
	int bubble, begin;

	void LogicProc(int W_stat, int m_stat, int M_icode, int E_begin, int m_valM) {
		if (set<int> { SADR, SINS, SHLT }.count(m_stat) || set<int> { SADR, SINS, SHLT }.count(W_stat))
			bubble = 1;
		else if (M_icode == IRET && m_valM != E_begin) {
			bubble = 1;
		} else 
			bubble = 0;
	}

	int stat, icode, Cnd, valE, valA, dstE, dstM;
	void Print(char *buf) {
		sprintf(buf, "%s\"M\" : { ",buf);
		sprintf(buf, "%s\"begin:\" : 0x%x, ",buf ,begin);
		sprintf(buf, "%s\"bubble\" : %d, ",buf ,bubble);
		sprintf(buf, "%s\"stat\" : %d, ",buf ,stat);
		sprintf(buf, "%s\"icode\" : %d, ",buf ,icode);
		sprintf(buf, "%s\"Cnd\" : %d, ",buf ,Cnd);
		sprintf(buf, "%s\"valE\" : %d, ",buf ,valE);
		sprintf(buf, "%s\"valA\" : %d, ",buf ,valA);
		sprintf(buf, "%s\"dstE\" : %d, ",buf ,dstE);
		sprintf(buf, "%s\"dstM\" : %d ",buf ,dstM);
		sprintf(buf, "%s}\n", buf);
	}
};

struct E_ { 
	int bubble,begin;

	void LogicProc(int E_icode, int E_dstM, int e_Cnd, int d_srcA, int d_srcB, int M_icode, int E_begin, int m_valM) {
		if (E_icode == IJXX && !e_Cnd) 
			bubble = 1;
		else if (set<int>{ IMRMOVL, IPOPL }.count(E_icode) && set<int>{ d_srcA, d_srcB }.count(E_dstM) && E_dstM != RNONE)
			bubble = 1;
		else if (M_icode == IRET && m_valM != E_begin)
			bubble = 1;
		else 
			bubble = 0;
	}

	int stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB; 
	void Print(char *buf) {
		sprintf(buf, "%s\"E\" : { ", buf);
		sprintf(buf, "%s\"begin:\" : 0x%x, ", buf, begin);
		sprintf(buf, "%s\"bubble\" : %d, ", buf,bubble);
		sprintf(buf, "%s\"stat\" : %d, ", buf,stat);
		sprintf(buf, "%s\"icode\" : %d, ", buf,icode);
		sprintf(buf, "%s\"ifun\" : %d, ", buf,ifun);
		sprintf(buf, "%s\"valC\" : %d, ", buf,valC);
		sprintf(buf, "%s\"valA\" : %d, ", buf,valA);
		sprintf(buf, "%s\"valB\" : %d, ", buf,valB);
		sprintf(buf, "%s\"dstE\" : %d, ", buf,dstE);
		sprintf(buf, "%s\"dstM\" : %d, ", buf,dstM);
		sprintf(buf, "%s\"srcA\" : %d, ", buf,srcA);
		sprintf(buf, "%s\"srcB\" : %d ", buf,srcB);
		sprintf(buf, "%s}\n", buf);
	}
};

struct D_ { 
	int bubble, stall, begin;
	
	/*bool D_bubble =
	# Mispredicted branch
	(E_icode == IJXX && !e_Cnd) ||
	# Stalling at fetch while ret passes through pipeline
	# but not condition for a load/use hazard
	!(E_icode in { IMRMOVL, IPOPL } && E_dstM in { d_srcA, d_srcB }) &&
	  IRET in { D_icode, E_icode, M_icode };*/

	void LogicProc(int D_icode, int E_icode, int M_icode, int d_srcA, int d_srcB, int E_dstM, int e_Cnd, int E_begin, int m_valM) {
		if (E_icode == IJXX && !e_Cnd) bubble = 1;
		else if (M_icode == IRET && m_valM != E_begin) bubble = 1;
		else bubble = 0;

		if (set<int> { IMRMOVL, IPOPL }.count(E_icode) && set<int> { d_srcA, d_srcB }.count(E_dstM)) 
			stall = 1;
		else 
			stall = 0;
		return;
	}

	int stat, icode, ifun, rA, rB, valC, valP; 
	void Print(char *buf) {
		sprintf(buf, "%s\"D\" : { ", buf);
		sprintf(buf, "%s\"begin:\" : 0x%x, ", buf,begin);
		sprintf(buf, "%s\"stall\" : %d, ", buf,stall);
		sprintf(buf, "%s\"bubble\" : %d, ", buf,bubble);
		sprintf(buf, "%s\"stat\" : %d, ", buf,stat);
		sprintf(buf, "%s\"icode\" : %d, ", buf,icode);
		sprintf(buf, "%s\"ifun\" : %d, ", buf,ifun);
		sprintf(buf, "%s\"rA\" : %d, ", buf,rA);
		sprintf(buf, "%s\"rB\" : %d, ", buf,rB);
		sprintf(buf, "%s\"valC\" : %d, ", buf,valC);
		sprintf(buf, "%s\"valP\" : %d ", buf,valP);
		sprintf(buf, "%s}\n", buf);
	}	
};

struct F_ { 
	int stall;

	void LogicProc(int E_icode, int E_dstM, int D_icode, int M_icode, int d_srcA, int d_srcB) {
		if ((set<int> { IMRMOVL, IPOPL }.count(E_icode) && set<int> { d_srcA, d_srcB }.count(E_dstM)))
			stall = 1;
		else stall = 0;
	}

	int predPC,begin; 
	void Print(char *buf) {
		sprintf(buf, "%s\"F\" : { ", buf);
		sprintf(buf, "%s\"stall\" : %d, ", buf,stall);
		sprintf(buf, "%s\"predPC\" : %d ", buf,predPC);
		sprintf(buf, "%s}\n", buf);
	}
};

struct PIPE_ {
	int stat;
	// PipelineRegisters
	W_ W;
	M_ M;
	E_ E;
	D_ D;
	F_ F;
	
	// LogicalUnits
	SelectPC_  SelectPC;
	f_icode_ f_icode;
	Needregids_ Needregids;
	NeedvalC_ NeedvalC;
	Instrvalid_ Instrvalid;
	f_ifun_ f_ifun;
	PredictPC_ PredictPC;
	f_stat_ f_stat;
	d_dstE_ d_dstE;
	d_dstM_ d_dstM;
	d_srcA_ d_srcA;
	d_srcB_ d_srcB;
	FwdA_ FwdA;
	FwdB_ FwdB;
	ALUA_ ALUA;
	ALUB_ ALUB;
	ALUfun_ ALUfun;
	e_dstE_ e_dstE;
	SetCC_ SetCC;
	Addr_ Addr;
	Memread_ Memread;
	Memwrite_ Memwrite;
	m_stat_ m_stat;

	//HardwareUnits
	CallStack_ CallStack;
	Align_ Align;
	Instructions_ Instructions;
	PCinc_ PCinc;
	Split_ Split;
	RegisterFiles_ RegisterFiles;
	ALU_ ALU;
	CC_ CC;
	cond_ cond;
	Datamemory_ Datamemory;

/*struct W_ { int stat, icode, valE, valM, dstE, dstM; };

struct M_ { int stat, icode, Cnd, valE, valA, dstE, dstM; };

struct E_ { int stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB; };

struct D_ { int stat, icode, ifun, rA, rB, valC, valP; };

struct F_ { int predPC; };
*/

	void Print(char *buf) {
		sprintf(buf, "{");
		W.Print(buf);
		M.Print(buf);
		E.Print(buf);
		D.Print(buf);
		F.Print(buf);
		RegisterFiles.Print(buf);
        sprintf(buf, "%s\"stat\" : %d \n", buf, stat);
		sprintf(buf, "%s}\n", buf);
	}

	void Init() {
		stat = D.stat = E.stat = M.stat = W.stat = SAOK;
	}

	void LHProc(const PIPE_ &Pas) {
		// Writeback
		RegisterFiles = Pas.RegisterFiles;
		RegisterFiles.Write(Pas.W.dstM, Pas.W.valM, Pas.W.dstE, Pas.W.valE, Pas.W.icode);
	
		// Memory
		Addr.Proc(Pas.M.icode, Pas.M.valE, Pas.M.valA);
		Memread.Proc(Pas.M.icode);
		Memwrite.Proc(Pas.M.icode);
		Datamemory = Pas.Datamemory;
		Datamemory.Proc(Memread.Get(), Memwrite.Get(), Addr.Get(), Pas.M.valA);
		//printf("read:%d write:%d Addr:%d M_valA:%d\n",Memread.Get(), Memwrite.Get(), Addr.Get(),Pas.M.valA);
		//printf("%d\n",Datamemory.Val[1000]);
		m_stat.Proc(Pas.M.stat, Datamemory.dmem_error);
		
		// Execute
		ALUfun.Proc(Pas.E.icode, Pas.E.ifun);
		ALUA.Proc(Pas.E.icode, Pas.E.valC, Pas.E.valA);
		ALUB.Proc(Pas.E.icode, Pas.E.valB);
		SetCC.Proc(Pas.E.icode, Pas.W.stat, Pas.m_stat.Get());
		ALU.Proc(ALUA.Get(), ALUB.Get(), ALUfun.Get()); 
		CC = Pas.CC;
		CC.Proc(ALU.ZF, ALU.SF, ALU.OF, SetCC.Get());
		cond.Proc(Pas.E.ifun, CC.le, CC.l, CC.e, CC.ne, CC.ge, CC.g);
		e_dstE.Proc(Pas.E.icode, cond.Get(), Pas.E.dstE);
		
		// Decode
		d_dstE.Proc(Pas.D.icode, Pas.D.rB);
		d_dstM.Proc(Pas.D.icode, Pas.D.rA);
		d_srcA.Proc(Pas.D.icode, Pas.D.rA);
		d_srcB.Proc(Pas.D.icode, Pas.D.rB);
		// printf("d_srcA:%d\n",d_srcA.Get());
		// printf("d_srcB:%d\n",d_srcB.Get());
		RegisterFiles.Proc(d_srcA.Get(), d_srcB.Get());
		FwdA.Proc(Pas.D.icode, Pas.D.valP, d_srcA.Get(), RegisterFiles.d_rvalA,
				e_dstE.Get(), ALU.valE, Pas.M.dstE, Pas.M.valE, Pas.M.dstM,
				Datamemory.Get(), Pas.W.dstM, Pas.W.valM, Pas.W.dstE, Pas.W.valE);
		//printf("d_rvalB: %d\n",RegisterFiles.d_rvalB);
		FwdB.Proc(d_srcB.Get(), RegisterFiles.d_rvalB,
				e_dstE.Get(), ALU.valE, Pas.M.dstE, Pas.M.valE, Pas.M.dstM,
				Datamemory.Get(), Pas.W.dstM, Pas.W.valM, Pas.W.dstE, Pas.W.valE);
	
		// Fetch
		SelectPC.Proc(Pas.M.icode, Pas.M.Cnd, Pas.M.valA, Pas.W.icode, Pas.W.valM, Pas.F.predPC, CallStack.addr, Pas.M.begin);		
		//Instructions = Pas.Instructions;
		Instructions.Proc(Pas.Datamemory.Val + SelectPC.Get(), SelectPC.Get());
		Split.Proc(Instructions.icode, Instructions.ifun);
		f_icode.Proc(Instructions.imem_error, Split.icode);
		f_ifun.Proc(Instructions.imem_error, Split.ifun);
		Instrvalid.Proc(f_icode.Get());
		f_stat.Proc(f_icode.Get(), Instrvalid.Get(), Instructions.imem_error);
		NeedvalC.Proc(f_icode.Get());
		Needregids.Proc(f_icode.Get());
		PCinc = Pas.PCinc;
		PCinc.Proc(SelectPC.Get(), Datamemory.Val);
		Align = Pas.Align;
		Align.Proc(Instructions.rA, Instructions.rB, Instructions.valC, Needregids.Get());	
		CallStack = Pas.CallStack;
		CallStack.Recover(SelectPC.step);
		CallStack.Proc(Split.icode, PCinc.Get());
		PredictPC.Proc(f_icode.Get(), Align.valC, PCinc.valP, CallStack.addr);
		D.begin = SelectPC.Get();
	}

	void ConveyF(const PIPE_ &Pas) {
	// To F
		//void LogicProc(int E_icode, int E_dstM, int D_icode, int M_icode, int d_srcA, int d_srcB) {
		F.LogicProc(Pas.E.icode, Pas.E.dstM, Pas.D.icode, Pas.M.icode, d_srcA.Get(), d_srcB.Get());
		if (F.stall) { F = Pas.F; F.stall = 1; return; }
		F.predPC = PredictPC.Get();
	}

	void ConveyD(const PIPE_ &Pas) {
		// To D
		//void LogicProc(int D_icode, int E_icode, int M_icode, int d_srcA, int d_srcB, int E_dstM, int e_Cnd) {
		D.LogicProc(Pas.D.icode, Pas.E.icode, Pas.M.icode, d_srcA.Get(), d_srcB.Get(), Pas.E.dstM, cond.Get(), Pas.E.begin, Datamemory.Get());
		if (D.stall) { D = Pas.D; D.stall = 1; D.bubble = 0; return; }
		if (D.bubble) { 
			D.begin = -1;
			D.icode = INOP;
			D.stat = SAOK;
			D.ifun = D.valC = D.valP = 0;
			D.rA = D.rB = RNONE;
			return;
		}
		D.stat = f_stat.Get();
		D.icode = f_icode.Get();
		D.ifun = f_ifun.Get();
		D.rA = Align.rA;
		D.rB = Align.rB;
		D.valC = Align.valC;
		D.valP = PCinc.valP;
	}
	
	void ConveyE(const PIPE_ &Pas) {
		// To E
		//void LogicProc(int E_icode, int E_dstM, int e_Cnd, int d_srcA, int d_srcB) {
		E.LogicProc(Pas.E.icode, Pas.E.dstM, cond.Get(), d_srcA.Get(), d_srcB.Get(), Pas.M.icode, Pas.E.begin, Datamemory.Get());
		if (E.bubble) {
			E.begin = -1;
			E.stat = SAOK;
			E.icode = INOP;
			E.dstE = E.dstM = E.srcA = E.srcB = RNONE;
			E.ifun = E.valC = 0;
			return;
		}
		E.stat = Pas.D.stat;
		E.icode = Pas.D.icode;
		E.ifun = Pas.D.ifun;
		E.valC = Pas.D.valC;
		E.valA = FwdA.Get();
		E.valB = FwdB.Get();
		E.dstE = d_dstE.Get();
		E.dstM = d_dstM.Get();
		E.srcA = d_srcA.Get();
		E.srcB = d_srcB.Get();
		E.begin = Pas.D.begin;
	}

	void ConveyM(const PIPE_ &Pas) {
		// To M
		//void LogicProc(int W_stat, int m_stat) {
		M.LogicProc(Pas.W.stat, m_stat.Get(), Pas.M.icode, Pas.E.begin, Datamemory.Get());
		if (M.bubble) {
			M.begin = -1;
			M.stat = SAOK;
			M.icode = INOP;
			M.dstE = M.dstM = RNONE;
			M.Cnd = M.valE = M.valA = 0;
			return;
		}
		M.stat = Pas.E.stat;
		M.icode = Pas.E.icode;
		M.Cnd = cond.Get();
		M.valE = ALU.valE;
		M.valA = Pas.E.valA;
		M.dstE = e_dstE.Get();
		M.dstM = Pas.E.dstM;
		M.begin = Pas.E.begin;
	}

	void ConveyW(const PIPE_ &Pas) {
		// To W
		//void LogicProc(int W_stat) {
		W.LogicProc(Pas.W.stat);
		if (W.stall) { 
			W = Pas.W;
			W.stall = 1;
			return;
		}
		W.stat = m_stat.Get();
		W.icode = Pas.M.icode;
		W.valE = Pas.M.valE;
		W.valM = Datamemory.Get();
		W.dstE = Pas.M.dstE;
		W.dstM = Pas.M.dstM;
		W.begin = Pas.M.begin;
	}

	void Convey(const PIPE_ &Pas) {
		ConveyF(Pas);
		ConveyD(Pas);
		ConveyE(Pas);
		ConveyM(Pas);
		ConveyW(Pas);
		stat = W.stat;
	}

	void Proc(const PIPE_ &Pas) {
		LHProc(Pas);
		Convey(Pas);
	}
};

extern PIPE_ PIPE[1110];

extern int clock_cnt, ended;

void pipe_init(const string &input);
void proc_one();
#endif //PIPE_H

