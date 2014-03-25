
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_value() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	stringstream msg;
	msg << "No get_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	stringstream msg;
	msg << "No set_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

// GOTO_AST ast

GOTO_AST::GOTO_AST( int tp)
{
		num = tp;
}

GOTO_AST::~GOTO_AST()
{
	num = 0;
}

Data_Type GOTO_AST::get_data_type()
{
	return node_data_type;
}

bool GOTO_AST::check_ast()
{
	
}

void GOTO_AST::print(ostream & file_buffer)
{
	file_buffer << AST_SPACE << "Goto statement:\n";

	file_buffer<< AST_NODE_SPACE<<"Successor: "<<num<<endl;
}

Eval_Result & GOTO_AST::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	print(file_buffer);
	file_buffer << "\n" << AST_SPACE << "GOTO (BB "<<num<<")";
	
	Eval_Result & result = *new Eval_Result_Value_Int();
	result.set_value(num);
	// Setting the result_enum as the basic block pointer
	// This setting would be useful in writing the procedure.cc code for changing the basic block pattern
	// basic_block_pointer will e checked for evaluation in the procedure.cc code
	result.set_result_enum(Result_Enum::basic_block_pointer);
	return result;
}

Code_For_Ast & GOTO_AST::compile()
{
	
    Icode_Stmt * goto_stmt = NULL;

    goto_stmt = new GOTO_IC_Stmt(jump,num);
    list<Icode_Stmt *> ic_list;
	
	ic_list.push_back(goto_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, machine_dscr_object.spim_register_table[zero]);

	return *assign_stmt;

}

Code_For_Ast & GOTO_AST::compile_and_optimize_ast(Lra_Outcome & lra)
{
	Icode_Stmt * goto_stmt = NULL;

    goto_stmt = new GOTO_IC_Stmt(jump,num);
    list<Icode_Stmt *> ic_list;
	
	ic_list.push_back(goto_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, machine_dscr_object.spim_register_table[zero]);

	return *assign_stmt;
}
////////////////////////////////////////////////////////////////
// If else ast

If_Else_Ast::If_Else_Ast(Ast * condp, int tp, int fp)
{
		cond = condp;
		t = tp;
		f = fp;
}

If_Else_Ast::~If_Else_Ast()
{
		delete cond;
}

Data_Type If_Else_Ast::get_data_type()
{
	return node_data_type;
}

bool If_Else_Ast::check_ast()
{
	
}

void If_Else_Ast::print(ostream & file_buffer)
{
	file_buffer << AST_SPACE << "If_Else statement:";
	cond->print(file_buffer);
	file_buffer << "\n";

	file_buffer<< AST_NODE_SPACE<<"True Successor: "<<t<<endl;
	file_buffer<< AST_NODE_SPACE<<"False Successor: "<<f<<endl;
}

Eval_Result & If_Else_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	file_buffer<<"\n";
	print(file_buffer);
	
	Eval_Result &cond_res = cond->evaluate(eval_env, file_buffer);
	Eval_Result* result = new Eval_Result_Value_Int();
	
	result->set_result_enum(Result_Enum::basic_block_pointer);
	
	int cond_val = cond_res.get_int_value();
	
	if(cond_val == 0) {
		result->set_value(f);
		file_buffer<<"\n"<<AST_SPACE<<"Condition False : Goto (BB "<<f<<")";
	}
	 else {
		result->set_value(t);
		file_buffer<<"\n"<<AST_SPACE<<"Condition True : Goto (BB "<<t<<")";
	}
	
	return *result;
}

Code_For_Ast & If_Else_Ast::compile()
{
	CHECK_INVARIANT((cond != NULL), "cond cannot be null");

	Code_For_Ast & compare = cond->compile();

	Register_Descriptor * load_register_c = compare.get_reg();

	Ics_Opd * register_opd_c = new Register_Addr_Opd(load_register_c);
	
	load_register_c->clear_lra_symbol_list();
    Icode_Stmt * branch_stmt = NULL;
    Icode_Stmt * goto_stmt = NULL;

    branch_stmt = new If_else_IC_Stmt(bne,register_opd_c, t);
    goto_stmt = new GOTO_IC_Stmt(jump,f);
    list<Icode_Stmt *> ic_list;
	
	if (compare.get_icode_list().empty() == false)
		ic_list = compare.get_icode_list();

	ic_list.push_back(branch_stmt);
	ic_list.push_back(goto_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register_c);

	return *assign_stmt;
}

Code_For_Ast & If_Else_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((cond != NULL), "cond cannot be null");

	Code_For_Ast & compare = cond->compile_and_optimize_ast(lra);

	Register_Descriptor * load_register_c = compare.get_reg();

	Ics_Opd * register_opd_c = new Register_Addr_Opd(load_register_c);
	
	load_register_c->clear_lra_symbol_list();
    Icode_Stmt * branch_stmt = NULL;
    Icode_Stmt * goto_stmt = NULL;

    branch_stmt = new If_else_IC_Stmt(bne,register_opd_c, t);
    goto_stmt = new GOTO_IC_Stmt(jump,f);
    list<Icode_Stmt *> ic_list;
	
	if (compare.get_icode_list().empty() == false)
		ic_list = compare.get_icode_list();

	ic_list.push_back(branch_stmt);
	ic_list.push_back(goto_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register_c);

	return *assign_stmt;
}
////////////////////////////////////////////////////////////////
/*Comparison Statement*/

Comparison_Ast::Comparison_Ast(Ast * temp_lhs, Ast * temp_rhs, int temp_comp)
{
		lhs = temp_lhs;
		rhs = temp_rhs;
		if(temp_comp == 0) {comp = GE;}
		if(temp_comp == 1) {comp = GT;}
		if(temp_comp == 2) {comp = LE;}
		if(temp_comp == 3) {comp = LT;}
		if(temp_comp == 4) {comp = EQ;}
		if(temp_comp == 5) {comp = NE;}
}

Comparison_Ast::~Comparison_Ast()
{
		delete lhs;
		delete rhs;
}

Data_Type Comparison_Ast::get_data_type()
{
	return node_data_type;
}

bool Comparison_Ast::check_ast()
{
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"Comparison data type not compatible");
}

void Comparison_Ast::print(ostream & file_buffer)
{
	file_buffer <<"\n";
	file_buffer << AST_NODE_SPACE << "Condition: ";
	if(comp == 0) file_buffer << "GE\n";
	if(comp == 1) file_buffer << "GT\n";
	if(comp == 2) file_buffer << "LE\n";
	if(comp == 3) file_buffer << "LT\n";
	if(comp == 4) file_buffer << "EQ\n";
	if(comp == 5) file_buffer << "NE\n";

	file_buffer << AST_SUB_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";

	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Comparison_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	
	Eval_Result & lhs_result = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhs_result = rhs->evaluate(eval_env, file_buffer);
	int lhs_res = lhs_result.get_int_value();
	int rhs_res = rhs_result.get_int_value();
	
	int res = 0;

	if(comp == 0) res = lhs_res >= rhs_res;
	if(comp == 1) res = lhs_res > rhs_res;
	if(comp == 2) res = lhs_res <= rhs_res;
	if(comp == 3) res = lhs_res < rhs_res;
	if(comp == 4) res = lhs_res == rhs_res;
	if(comp == 5) res = lhs_res != rhs_res;
	
	Eval_Result& result = *new Eval_Result_Value_Int();
	result.set_value(res);
	return result;
}

Code_For_Ast & Comparison_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Code_For_Ast & lsl = lhs->compile();
	Code_For_Ast & lsr = rhs->compile();


	Register_Descriptor * load_register_l = lsl.get_reg();
	Register_Descriptor * load_register_r = lsr.get_reg();

	Ics_Opd * register_opd_1 = new Register_Addr_Opd(load_register_l);
	Ics_Opd * register_opd_2 = new Register_Addr_Opd(load_register_r);

	Register_Descriptor * register_r = machine_dscr_object.get_new_register();
	Symbol_Table_Entry & sym_entry = * new Symbol_Table_Entry();
	register_r->update_symbol_information(sym_entry);

	Ics_Opd * register_result = new Register_Addr_Opd(register_r);
	load_register_l->clear_lra_symbol_list();
    load_register_r->clear_lra_symbol_list();
    Icode_Stmt * load_stmt = NULL;

    if(comp == 0) load_stmt = new Comparison_IC_Stmt(sge, register_opd_1,register_opd_2, register_result);
    if(comp == 1) load_stmt = new Comparison_IC_Stmt(sgt, register_opd_1,register_opd_2, register_result);
    if(comp == 2) load_stmt = new Comparison_IC_Stmt(sle, register_opd_1,register_opd_2, register_result);
    if(comp == 3) load_stmt = new Comparison_IC_Stmt(slt, register_opd_1,register_opd_2, register_result);
    if(comp == 4) load_stmt = new Comparison_IC_Stmt(seq, register_opd_1,register_opd_2, register_result);
    if(comp == 5) load_stmt = new Comparison_IC_Stmt(sne, register_opd_1,register_opd_2, register_result);

    list<Icode_Stmt *> ic_list;
	
	if (lsl.get_icode_list().empty() == false)
		ic_list = lsl.get_icode_list();

	if (lsr.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lsr.get_icode_list());

	ic_list.push_back(load_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, register_r);

	return *assign_stmt;
}

Code_For_Ast & Comparison_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	if(typeid(*lhs) == typeid(Number_Ast<int>))
	{	
		// Optimising the LRA of the lhs with c2r as the reference
		lra.optimize_lra(c2r, lhs, lhs);
	}

	else if(typeid(*lhs) == typeid(Name_Ast))
	{
		// Optimising the LRA of the rhs with m2r as the reference
		lra.optimize_lra(m2r,lhs,lhs);
	}

	Code_For_Ast & lsl = lhs->compile_and_optimize_ast(lra);

	if(typeid(*rhs) == typeid(Number_Ast<int>))
	{
		lra.optimize_lra(c2r,rhs,rhs);
	}

	else if(typeid(*rhs) == typeid(Name_Ast))
	{
		lra.optimize_lra(m2r,rhs,rhs);
	}

	Code_For_Ast & lsr = rhs->compile_and_optimize_ast(lra);

	Register_Descriptor * load_register_l = lsl.get_reg();
	Register_Descriptor * load_register_r = lsr.get_reg();

	Ics_Opd * register_opd_1 = new Register_Addr_Opd(load_register_l);
	Ics_Opd * register_opd_2 = new Register_Addr_Opd(load_register_r);

	Register_Descriptor * register_r = machine_dscr_object.get_new_register();
	Symbol_Table_Entry & sym_entry = * new Symbol_Table_Entry();
	register_r->update_symbol_information(sym_entry);

	Ics_Opd * register_result = new Register_Addr_Opd(register_r);

    Icode_Stmt * load_stmt = NULL;
    list<Icode_Stmt *> ic_list;

    if(comp == 0) load_stmt = new Comparison_IC_Stmt(sge, register_opd_1,register_opd_2, register_result);
    if(comp == 1) load_stmt = new Comparison_IC_Stmt(sgt, register_opd_1,register_opd_2, register_result);
    if(comp == 2) load_stmt = new Comparison_IC_Stmt(sle, register_opd_1,register_opd_2, register_result);
    if(comp == 3) load_stmt = new Comparison_IC_Stmt(slt, register_opd_1,register_opd_2, register_result);
    if(comp == 4) load_stmt = new Comparison_IC_Stmt(seq, register_opd_1,register_opd_2, register_result);
    if(comp == 5) load_stmt = new Comparison_IC_Stmt(sne, register_opd_1,register_opd_2, register_result);

    	if(typeid(*lhs) == typeid(Number_Ast<int>) || typeid(*lhs) == typeid(Comparison_Ast))
    	{
			load_register_l->clear_lra_symbol_list();
		}
	
		if(typeid(*lhs) == typeid(Name_Ast) && !(load_register_l->get_use_for_expr_result()))
		{
			(lhs->get_symbol_entry()).free_register(load_register_l);
		}
    
    	if(typeid(*rhs) == typeid(Number_Ast<int>) || typeid(*rhs) == typeid(Comparison_Ast))
    	{
			load_register_r->clear_lra_symbol_list();
		}
	
		if(typeid(*rhs) == typeid(Name_Ast) && !(load_register_r->get_use_for_expr_result()))
		{
			(rhs->get_symbol_entry()).free_register(load_register_r);
		}

	
	if (lsl.get_icode_list().empty() == false)
		ic_list = lsl.get_icode_list();

	if (lsr.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lsr.get_icode_list());

	ic_list.push_back(load_stmt);

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, register_r);

	return *assign_stmt;
}


////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"Assignment statement data type not compatible");
}

void Assignment_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Asgn:";

	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(result.is_variable_defined(), "Variable should be defined to be on rhs of Assignment_Ast", lineno);

	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	lhs->set_value_of_evaluation(eval_env, result);

	// Print the result

	print(file_buffer);

	lhs->print_value(eval_env, file_buffer);

	return result;
}

Code_For_Ast & Assignment_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	load_register->clear_lra_symbol_list();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	if(typeid(*rhs) == typeid(Name_Ast) || typeid(*rhs) == typeid(Number_Ast<int>))
		lra.optimize_lra(mc_2m, lhs, rhs);

	Register_Descriptor * register_s;

	if(typeid(*rhs) == typeid(Comparison_Ast))
		register_s = (lhs->get_symbol_entry()).get_register();

	Code_For_Ast load_stmt = rhs->compile_and_optimize_ast(lra);

	Register_Descriptor * result_register = load_stmt.get_reg();

	Code_For_Ast store_stmt = lhs->create_store_stmt(result_register);

	list<Icode_Stmt *> ic_list;

	if(typeid(*rhs) == typeid(Comparison_Ast))
	{
		if(register_s != NULL)
			register_s->clear_lra_symbol_list();
		lhs->get_symbol_entry().set_register(NULL);
		result_register->update_symbol_information(lhs->get_symbol_entry());
		lhs->get_symbol_entry().set_register(result_register);
	}
	else
	{
		result_register->set_use_for_expr_result();
	}

	//cout<<result_register->get_name()<<endl;

	if(result_register->is_free())
	{
		//cout<<"It is free"<<endl;
	}

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	return *assign_stmt;
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	variable_symbol_entry = &var_entry;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");

	ast_num_child = zero_arity;
	node_data_type = variable_symbol_entry->get_data_type();
	lineno = line;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return *variable_symbol_entry;
}

void Name_Ast::print(ostream & file_buffer)
{
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << "\n" << AST_SPACE << variable_name << " : ";

	if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
		file_buffer << "undefined";

	else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
		CHECK_INVARIANT(loc_var_val->get_result_enum() == int_result, "Result type can only be int");
		file_buffer << loc_var_val->get_int_value() << "\n";
	}

	else
	{
		CHECK_INVARIANT(glob_var_val->get_result_enum() == int_result, 
			"Result type can only be int and float");

		if (glob_var_val == NULL)
			file_buffer << "0\n";
		else
			file_buffer << glob_var_val->get_int_value() << "\n";
	}
	file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	if (eval_env.does_variable_exist(variable_name))
	{
		CHECK_INPUT_AND_ABORT((eval_env.is_variable_defined(variable_name) == true), 
					"Variable should be defined before its use", lineno);

		Eval_Result * result = eval_env.get_variable_value(variable_name);
		return *result;
	}

	CHECK_INPUT_AND_ABORT((interpreter_global_table.is_variable_defined(variable_name) == true), 
				"Variable should be defined before its use", lineno);

	Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
	return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	Eval_Result * i;
	string variable_name = variable_symbol_entry->get_variable_name();

	if (variable_symbol_entry->get_data_type() == int_data_type)
		i = new Eval_Result_Value_Int();
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (result.get_result_enum() == int_result)
	 	i->set_value(result.get_int_value());
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (eval_env.does_variable_exist(variable_name))
		eval_env.put_variable_value(*i, variable_name);
	else
		interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	return get_value_of_evaluation(eval_env);
}

Code_For_Ast & Name_Ast::compile()
{
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(load, opd, register_opd);

	if (command_options.is_do_lra_selected() == false)
		variable_symbol_entry->update_register(result_register);

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");

	Ics_Opd * register_opd = new Register_Addr_Opd(store_register);
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

	Icode_Stmt * store_stmt = new Move_IC_Stmt(store, register_opd, opd);

	if (command_options.is_do_lra_selected() == false)
		variable_symbol_entry->free_register(store_register);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(store_stmt);

	Code_For_Ast & name_code = *new Code_For_Ast(ic_list, store_register);

	return name_code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;;

	bool load_needed = lra.is_load_needed();

	Register_Descriptor * result_register = lra.get_register();
	CHECK_INVARIANT((result_register != NULL), "Register cannot be null");
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = NULL;
	if (load_needed)
	{
		Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

		load_stmt = new Move_IC_Stmt(load, opd, register_opd);
			
		ic_list.push_back(load_stmt);
	}

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	constant = number;
	node_data_type = constant_data_type;

	ast_num_child = zero_arity;

	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	file_buffer << std::fixed;
	file_buffer << std::setprecision(2);

	file_buffer << "Num : " << constant;
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_value(constant);

		return result;
	}
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	
	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");
	
	Symbol_Table_Entry & sym_entry = * new Symbol_Table_Entry();
	result_register->update_symbol_information(sym_entry);

	Ics_Opd * load_register = new Register_Addr_Opd(result_register);
	Ics_Opd * opd = new Const_Opd<int>(constant);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, result_register);

	return num_code;
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lra.get_register() != NULL), "Register assigned through optimize_lra cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(lra.get_register());
	Ics_Opd * opd = new Const_Opd<int>(constant);

	Register_Descriptor * result_register = lra.get_register();
	Symbol_Table_Entry & sym_entry = * new Symbol_Table_Entry();
	result_register->update_symbol_information(sym_entry);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, lra.get_register());

	return num_code;
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(int line)
{
	lineno = line;
	node_data_type = void_data_type;
	ast_num_child = zero_arity;
}

Return_Ast::~Return_Ast()
{}

void Return_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Return <NOTHING>\n";
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result & result = *new Eval_Result_Value_Int();
	return result;
}

Code_For_Ast & Return_Ast::compile()
{
	Code_For_Ast & ret_code = *new Code_For_Ast();
	return ret_code;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	Code_For_Ast & ret_code = *new Code_For_Ast();
	return ret_code;
}

template class Number_Ast<int>;
