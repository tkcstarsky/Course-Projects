
#include "assembly_builder.h"

#include <vector>

#define ConstNum(n) ConstantInt::get(context, APInt(32, n))

using namespace llvm;
using namespace c1_recognizer::syntax_tree;

void assembly_builder::visit(assembly &node)
{
    // Init the global_defs 
    ptr_list<global_def_syntax> glo_defs = node.global_defs;

	//give every global_def a init value (int 0)
	for (auto &glo_def : glo_defs) {
		value_result = ConstNum(0);
		lval_as_rval = true;
		constexpr_expected = false;
		in_global = true;
		glo_def->accept(*this);
	}
}

void assembly_builder::visit(func_def_syntax &node)
{
    in_global = false;
	
	// Error check : check if this function has been defined
	if (functions.count(node.name)) {
		err.error(node.line, node.pos, "Function '" + node.name + "' had been defined. Skipping.");
		error_flag = true;
		return;
	}
	
    // Put in the function name 
	FunctionType *FuncType = FunctionType::get(Type::getVoidTy(context), {}, false);
	current_function = Function::Create(FuncType, GlobalValue::LinkageTypes::ExternalLinkage, node.name, module.get());
	functions.emplace(node.name, current_function);     // Add new func_name to the func_name vector

    // Create a new BB
	auto enterBlock = BasicBlock::Create(context, "Enter", current_function);
	builder.SetInsertPoint(enterBlock);

    // Begin to accept the body code
	node.body->accept(*this);

    // BB return
	builder.CreateRetVoid();
}

void assembly_builder::visit(cond_syntax &node)
{
	//Var two Value to get lhs & rhs
    Value *tmp_lhs, *tmp_rhs;
	//Check cond 
	bool is_int_lhs,is_int_rhs;
	node.lhs->accept(*this);
	is_int_lhs = is_result_int;
	tmp_lhs = value_result;
	node.rhs->accept(*this);
	is_int_rhs = is_result_int;
	tmp_rhs = value_result;
	// Error check : statement left and right type not equal
	if (tmp_lhs->getType() != tmp_rhs->getType()) {
		err.error(node.line, node.pos, "statement left and right type not equal!");
		error_flag = true;
	}

	// Create each op result
	if (node.op == relop::equal) {
		// Error check: float can't use equal
		if((is_int_lhs==false)||(is_int_rhs==false)){
			err.error(node.line, node.pos, "float can't use equal!");
			error_flag = true;
		}
		value_result = builder.CreateICmpEQ(tmp_lhs, tmp_rhs);
	}
	else if (node.op == relop::greater) {
		value_result = builder.CreateICmpSGT(tmp_lhs, tmp_rhs);
	}
	else if (node.op == relop::greater_equal) {
		value_result = builder.CreateICmpSGE(tmp_lhs, tmp_rhs);
	}
	else if(node.op == relop::less) {
		value_result = builder.CreateICmpSLT(tmp_lhs, tmp_rhs);
	}
	else if(node.op == relop::less_equal) {
		
		value_result = builder.CreateICmpSLE(tmp_lhs, tmp_rhs);
	}
	else if(node.op == relop::non_equal) {
		// Error check: float can't use non_equal
		if((is_int_lhs==false)||(is_int_rhs==false)){
			err.error(node.line, node.pos, "float can't use non_equal!");
			error_flag = true;
		}
		value_result = builder.CreateICmpNE(tmp_lhs, tmp_rhs);
	}
}

void assembly_builder::visit(binop_expr_syntax &node)
{
    if (constexpr_expected == false) {
		// find lower stage exp
		Value *tmp_lhs, *tmp_rhs;
		node.lhs->accept(*this);
		if (value_result == nullptr) {
			return;
		}
		tmp_lhs = value_result;
		node.rhs->accept(*this);
		if (value_result == nullptr) {
			return;
		}
		tmp_rhs = value_result;

		if (node.op == binop::plus) {
			value_result = builder.CreateAdd(tmp_lhs, tmp_rhs);
		}
		else if (node.op == binop::minus) {
			value_result = builder.CreateSub(tmp_lhs, tmp_rhs);
		}
		else if (node.op == binop::multiply) {
			value_result = builder.CreateMul(tmp_lhs, tmp_rhs);
		}
		else if (node.op == binop::divide) {
			value_result = builder.CreateSDiv(tmp_lhs, tmp_rhs);
		}
		else if (node.op == binop::modulo) {
			value_result = builder.CreateSRem(tmp_lhs, tmp_rhs);
		}
	}
	else {
		// give exp actural value
		int int_tmp_lhs, int_tmp_rhs;
        double float_tmp_lhs, float_tmp_rhs;
        bool tmpl,tmpr;
		node.lhs->accept(*this);
		if (value_result == nullptr) {
			return;
		}
		int_tmp_lhs = int_const_result;
        float_tmp_lhs = float_const_result;
        tmpl = is_result_int;
		node.rhs->accept(*this);
		if (value_result == nullptr) {
			return;
		}
		int_tmp_rhs = int_const_result;
        float_tmp_rhs = float_const_result;
        tmpr = is_result_int;

		// Two int op
        if((tmpl==1)&&(tmpr==1))
        {
		    is_result_int = 1;
            if (node.op == binop::plus) {
			    int_const_result = int_tmp_lhs + int_tmp_rhs;
		    }
		    else if (node.op == binop::minus) {
			    int_const_result = int_tmp_lhs - int_tmp_rhs;
		    }
		    else if (node.op == binop::multiply) {
			    int_const_result = int_tmp_lhs * int_tmp_rhs;
		    }
		    else if (node.op == binop::divide) {
			// Error check : if the divided number is 0.
			    if (int_tmp_rhs == 0) {
				    err.error(node.rhs->line, node.rhs->pos, "Divided number is 0");
				    error_flag = true;
				    value_result = nullptr;
				    return;
			    }
			    int_const_result = int_tmp_lhs / int_tmp_rhs;
		    }
		    else if (node.op == binop::modulo) {
			// Error check : if the moduloed number is 0.
			if (int_tmp_rhs == 0) {
				err.error(node.rhs->line, node.rhs->pos, "Moduloed number is 0.");
				error_flag = true;
				value_result = nullptr;
				return;
			    }
			    int_const_result = int_tmp_lhs % int_tmp_rhs;
		    }
        } else {
			// 1~2 double op (trans the int to the double)
            is_result_int = 0;

			if(tmpl==1) float_tmp_lhs =(double)int_tmp_lhs;
			if(tmpr==1) float_tmp_rhs =(double)int_tmp_rhs;

            if (node.op == binop::plus) {
			    float_const_result = float_tmp_lhs + float_tmp_rhs;
		    }
		    else if (node.op == binop::minus) {
			    float_const_result = float_tmp_lhs - float_tmp_rhs;
		    }
		    else if (node.op == binop::multiply) {
			    float_const_result = float_tmp_lhs * float_tmp_rhs;
		    }
		    else if (node.op == binop::divide) {
			    // Error check : if the divided number is 0.
			    if (float_tmp_rhs == 0) {
				    err.error(node.rhs->line, node.rhs->pos, "Divided number is zero!");
				    error_flag = true;
				    value_result = nullptr;
				    return;
			    }
			    float_const_result = float_tmp_lhs / float_tmp_rhs;
		    }
		    else if (node.op == binop::modulo) {
			    // Error check : Const float can't modulo.
				err.error(node.lhs->line, node.lhs->pos, "Const float can't modulo!");
				error_flag = true;
				value_result = nullptr;
				return;
			}
        }

	}
}

void assembly_builder::visit(unaryop_expr_syntax &node)
{
    if (constexpr_expected == false) {
		// into value_result
		node.rhs->accept(*this);
		if (value_result == nullptr) {
			return;
		}
		if (node.op == unaryop::minus) {
			value_result = builder.CreateNeg(value_result);
		}
	}
	else {
		// into constexpr
		node.rhs->accept(*this);
		if (value_result == nullptr) {
			return;
		}
		if (node.op == unaryop::minus) {
			int_const_result = -int_const_result;
            float_const_result = -float_const_result;
		}
	}
}

void assembly_builder::visit(lval_syntax &node)
{
    if (std::get<0>(lookup_variable(node.name)) == nullptr) {
		// Error check:if use undeclared variable
		err.error(node.line, node.pos, "Use undeclared variable '" + node.name + "'.");
		if (constexpr_expected == true) {
			int_const_result = 0;
		}
		error_flag = true;
		value_result = nullptr;
		return;
	}

	if (lval_as_rval == false) {
		// go into lval
		if (node.array_index == nullptr) {
			// not an array 
			value_result = std::get<0>(lookup_variable(node.name));
		}
		else {
			// array
			auto tmp_const_exp = constexpr_expected;
			constexpr_expected = false;
			auto tmp_lval = lval_as_rval;
			lval_as_rval = true;
			node.array_index->accept(*this);
			if (value_result == nullptr) {
				return;
			}
			lval_as_rval = tmp_lval;
			constexpr_expected = tmp_const_exp;
			Value* indexList[] = { ConstNum(0), value_result };
			auto id_ptr = std::get<0>(lookup_variable(node.name));
			value_result = builder.CreateGEP(id_ptr, indexList);
		}
	}
	else if(constexpr_expected == false){
		// go into rval
		if (node.array_index == nullptr) {
			// not an array

			// Error : check if is_array
			if (std::get<2>(lookup_variable(node.name)) == true) {
				// is_array
				err.error(node.line, node.pos, "Array used as integer variable: '" + node.name + "'.");
				error_flag = true;
				value_result = nullptr;
				return;
			}
			
			auto id_ptr = std::get<0>(lookup_variable(node.name));
			//if(is_result_int)
				value_result = builder.CreateLoad(Type::getInt32Ty(context), id_ptr);
			//else 
				//value_result = builder.CreateLoad(Type::getDoubleTy(context), id_ptr);
		}
		else {
			// array
			// Error check :if is_array
			if (std::get<2>(lookup_variable(node.name)) == false) {
				err.error(node.line, node.pos, "Subscripted value is not an array: '" + node.name + "'.");
				error_flag = true;
				value_result = nullptr;
				return;
			}

			node.array_index->accept(*this);
			Value* indexList[] = {ConstNum(0),  value_result};
			auto id_ptr = std::get<0>(lookup_variable(node.name));
			value_result = builder.CreateGEP(id_ptr, indexList);
			value_result = builder.CreateLoad(Type::getInt32Ty(context), value_result);
		}
	}
	else {
		// Error check : need constant expression 
		err.error(node.line, node.pos, "Constant expression expected: '" + node.name + "'.");
		value_result = nullptr;
		int_const_result = 0;
		error_flag = true;
	}
}

void assembly_builder::visit(literal_syntax &node)
{
    is_result_int = node.is_int;
    if (constexpr_expected == false) {
		// go into value_result
        if(is_result_int)
		    value_result = ConstantInt::get(Type::getInt32Ty(context), node.intConst);
        else
            value_result = ConstantFP::get(Type::getDoubleTy(context), node.floatConst);
	}
	else {
		// go into constexpr
        int_const_result = 0;
        float_const_result = 0;
        if(is_result_int)
		    int_const_result = node.intConst;
        else
            float_const_result = node.floatConst;
	}
}

void assembly_builder::visit(var_def_stmt_syntax &node)
{
    // Error check : check multiple declaration
	if (variables[0].count(node.name)) {
		err.error(node.line, node.pos, "There are multiple declarations of variable '" + node.name + "' in current scope.");
		error_flag = true;
		return;
	}

	Value *ca, *index;
	if (in_global == false) {
		// local
		if (node.array_length == nullptr) { 
			// non-array
			constexpr_expected = false;
            if(node.is_int==true)
			    ca = builder.CreateAlloca(Type::getInt32Ty(context), nullptr, node.name);
            else 
                ca = builder.CreateAlloca(Type::getDoubleTy(context), nullptr, node.name);
			if (node.initializers.size() != 0) {
				node.initializers[0]->accept(*this);
				if (value_result == nullptr) {
					return;
				}
				builder.CreateStore(value_result, ca);
			}
		}
		else {
			// array
			constexpr_expected = true;
			auto tmp_err = error_flag;
			error_flag = false;
			auto check_but_dump = false;
			node.array_length->accept(*this);
			value_result = ConstNum(int_const_result);
			constexpr_expected = false;
            
			// Error check : if too much init.
			if (int_const_result < node.initializers.size()) {
				err.error(node.line, node.pos, "Initializer expressions more than array's length");
				error_flag = true;
			}
			// Error check : if negative length.
			if (int_const_result <= 0) {
				err.error(node.line, node.pos, "Not a const positive array length.");
				error_flag = true;
			}
			if (error_flag) {
				check_but_dump = true;
			}
			else {
				error_flag = tmp_err;
			}
			if (check_but_dump)
				return;

            // Error : check if array_length is interger
            if(is_result_int==true)
			    ca = builder.CreateAlloca(ArrayType::get(Type::getInt32Ty(context), int_const_result), ConstNum(1), node.name);
            else{
                err.error(node.line, node.pos, "Array's length must be interger.");
				error_flag = true;
            }
                
			int i = 0;
			for (auto &init : node.initializers) {
				Value* indexList[] = {ConstNum(0), ConstNum(i++) };
				index = builder.CreateGEP(ca, indexList);
				init->accept(*this);
				builder.CreateStore(value_result, index);
			}
		}
	}
	else {
		// global
		constexpr_expected = true;
		if (node.array_length == nullptr) {
			// not an array
            if(node.is_int==true)
            {
			    ConstantInt *init;
			    if (node.initializers.size() == 0) {
				    init = ConstNum(0);
			    }
			    else {
				    node.initializers[0]->accept(*this);
				    if (value_result == nullptr) {
				    	return;
				    }
				    init = ConstNum(int_const_result);
			    }
			    ca = new GlobalVariable(*module.get(), Type::getInt32Ty(context), node.is_constant, GlobalValue::LinkageTypes::ExternalLinkage, init, node.name);
            } else {

			    Constant *initf;
			    if (node.initializers.size() == 0) {
				    initf = ConstantFP::get(Type::getDoubleTy(context), 0);
			    }
			    else {
				    node.initializers[0]->accept(*this);
				    if (value_result == nullptr) {
				    	return;
				    }
				    initf = ConstantFP::get(Type::getDoubleTy(context), float_const_result);
			    }
			    ca = new GlobalVariable(*module.get(), Type::getDoubleTy(context), node.is_constant, GlobalValue::LinkageTypes::ExternalLinkage, initf, node.name);
            }

		}
		else {
			// array
			auto tmp_err = error_flag;
			auto check_but_dump = false;
			lval_as_rval = true;
			error_flag = false;
			node.array_length->accept(*this);
			int len = int_const_result;
			
			// check if too much init.
			if (len < node.initializers.size()) {
				err.error(node.line, node.pos, "Initializer expressions more than array's length");
				error_flag = true;
			}
			// check if negative length.
			if (len <= 0) {
				err.error(node.line, node.pos, "Not a const positive array length.");
				error_flag = true;
			}
			// check array length is an interger
			if(is_result_int==false){
                err.error(node.line, node.pos, "Array's length must be interger.");
				error_flag = true;
            }
			
			// no matter init or not, init to 0!
			std::vector<Constant*> values;
			int i = 0;
			for (auto &init : node.initializers) {
				lval_as_rval = true;
				constexpr_expected = true;
				value_result = ConstNum(0);
				init->accept(*this);
				values.push_back(ConstNum(int_const_result));
			}
			for (int i = values.size(); i < len; i++) {
				values.push_back(ConstNum(0));
			}
			if (error_flag) {
				check_but_dump = true;
			}
			else {
				error_flag = tmp_err;
			}
			if (check_but_dump)
				return;
			auto init = ConstantArray::get(ArrayType::get(Type::getInt32Ty(context), len), values);

			ca = new GlobalVariable(*module.get(), ArrayType::get(Type::getInt32Ty(context), len), node.is_constant, GlobalValue::LinkageTypes::ExternalLinkage, init, node.name);
		}
	}
	declare_variable(node.name, ca, node.is_constant, node.array_length != nullptr,node.is_int);
}

void assembly_builder::visit(assign_stmt_syntax &node)
{
    // check if target exists.
	if (std::get<0>(lookup_variable(node.target->name)) == nullptr) {
		// not exists.
		err.error(node.target->line, node.target->pos, "Use of undeclared variable '" + node.target->name + "'.");
		error_flag = true;
		return;
	}

	// check if target is const.
	if (std::get<1>(lookup_variable(node.target->name)) == true) {
		// const
		err.error(node.target->line, node.target->pos, "Assigning to constant:'" + node.target->name + "'.");
		error_flag = true;
		return;
	}

	// check if target is used incorrectly.
	if (std::get<2>(lookup_variable(node.target->name)) == false && node.target->array_index ) {
		// int used as array.
		err.error(node.target->line, node.target->pos, "Subscripted value is not an array: '" + node.target->name +"'.");
		error_flag = true;
		return;
	}
	if (std::get<2>(lookup_variable(node.target->name)) == true && node.target->array_index == nullptr) {
		// array used as int.
		err.error(node.target->line, node.target->pos, "Array used as integer variable: '" + node.target->name + "'.");
		error_flag = true;
		return;
	}
	lval_as_rval = false;
	node.target->accept(*this);

	lval_as_rval = true;
	Value *target = value_result;
	value_result = ConstNum(0);
	node.value->accept(*this);

	Value *value = value_result;
	
	if (target == nullptr || value == nullptr) {
		return;
	}
	builder.CreateStore(value, target);
}

void assembly_builder::visit(func_call_stmt_syntax &node)
{
    // load function call 
	auto callee = functions[node.name];
	if (callee == nullptr) {
		// Error check : Called function is not defined.
		err.error(node.line, node.pos, "This called function not defined: '" + node.name + "'.");
		error_flag = true;
		return;
	}
	builder.CreateCall(callee, {});
}

void assembly_builder::visit(block_syntax &node)
{
    // use enter_scope() and init block
	enter_scope();
	ptr_list<stmt_syntax> body = node.body;
	for (auto &b : body) {
		value_result = ConstNum(0);
		lval_as_rval = true;
		constexpr_expected = false;
		b.get()->accept(*this);
	}
	exit_scope();
}

void assembly_builder::visit(if_stmt_syntax &node)
{
	// Create 4 BB to cur if code into 4 BB
    auto ifB = BasicBlock::Create(context, "IfB", current_function);
	auto thenB = BasicBlock::Create(context, "ThenB", current_function);
	auto elseB = BasicBlock::Create(context, "ElseB", current_function);
	auto afterIfB = BasicBlock::Create(context, "AfterIf", current_function);
	
	builder.CreateBr(ifB);
	builder.SetInsertPoint(ifB);
	node.pred->accept(*this);

	builder.CreateCondBr(value_result, thenB, elseB);
	builder.SetInsertPoint(thenB);
	node.then_body->accept(*this);

	builder.CreateBr(afterIfB);
	builder.SetInsertPoint(elseB);
	if (node.else_body) {
		node.else_body->accept(*this);
	}
	builder.CreateBr(afterIfB);
	builder.SetInsertPoint(afterIfB);
}

void assembly_builder::visit(while_stmt_syntax &node)
{
    // Create 3 BB to cur while code into 3 BB
	auto whileB = BasicBlock::Create(context, "WhileB", current_function);
	auto blockB = BasicBlock::Create(context, "BlockB", current_function);
	auto afterB = BasicBlock::Create(context, "AfterB", current_function);

	builder.CreateBr(whileB);
	builder.SetInsertPoint(whileB);
	node.pred->accept(*this);

	builder.CreateCondBr(value_result, blockB, afterB);
	builder.SetInsertPoint(blockB);
	node.body->accept(*this);

	builder.CreateBr(whileB);
	builder.SetInsertPoint(afterB);
}

void assembly_builder::visit(empty_stmt_syntax &node)
{
	//blank visit
}
