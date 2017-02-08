/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author Federico Bond <federicobond@gmail.com>
 * @date 2016
 * Static analyzer and checker.
 */

#pragma once

#include <libsolidity/analysis/TypeChecker.h>
#include <libsolidity/ast/Types.h>
#include <libsolidity/ast/ASTAnnotations.h>
#include <libsolidity/ast/ASTForward.h>
#include <libsolidity/ast/ASTVisitor.h>

namespace dev
{
namespace solidity
{


/**
 * The module that performs static analysis on the AST.
 */
class StaticAnalyzer: private ASTConstVisitor
{
public:
	/// @param _errors the reference to the list of errors and warnings to add them found during static analysis.
	explicit StaticAnalyzer(std::vector<Declaration const*> const& _globals, ErrorList& _errors):
		m_errors(_errors)
	{
		m_globals.reserve(_globals.size() + 2);
		m_globals.push_back("this");
		m_globals.push_back("super");
		for (Declaration const* declaration: _globals)
			m_globals.push_back(declaration->name());
	}

	/// Performs static analysis on the given source unit and all of its sub-nodes.
	/// @returns true iff all checks passed. Note even if all checks passed, errors() can still contain warnings
	bool analyze(SourceUnit const& _sourceUnit);

private:
	/// Adds a new warning to the list of errors.
	void warning(SourceLocation const& _location, std::string const& _description);
	void checkShadowingBuiltin(Declaration const& _declaration);

	virtual bool visit(ContractDefinition const& _contract) override;
	virtual void endVisit(ContractDefinition const& _contract) override;

	virtual bool visit(FunctionDefinition const& _function) override;
	virtual void endVisit(FunctionDefinition const& _function) override;

	virtual bool visit(StructDefinition const& _struct) override;
	virtual void endVisit(StructDefinition const& _struct) override;

	virtual bool visit(MemberAccess const& _memberAccess) override;
	virtual bool visit(VariableDeclaration const& _variable) override;

	virtual bool visit(EventDefinition const& _event) override;

	virtual bool visit(ImportDirective const& _import) override;

	std::vector<std::string> m_globals;

	ErrorList& m_errors;

	/// Flag that indicates whether the current contract definition is a library.
	bool m_library = false;

	/// Flag that indicates whether a public function does not contain the "payable" modifier.
	bool m_nonPayablePublic = false;

	/// Flag that indicates whether the current scope is within a struct definition.
	bool m_withinStruct = false;
};

}
}
