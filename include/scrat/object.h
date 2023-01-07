//Copyright(C) 2011; 2022  oldlonecoder / lussier.serge@gmail.com
//
//This program is free software; you can redistribute itand /or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.

#pragma once
#include <scrat/dlconfig.h>
#include <vector>

#include <scrat/result.h>

/*
 !! notes


 https://learn.microsoft.com/en-us/powershell/scripting/samples/creating-a-custom-input-box?view=powershell-7.3
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

$form = New-Object System.Windows.Forms.Form
$form.Text = 'Data Entry Form'
$form.Size = New-Object System.Drawing.Size(300,200)
$form.StartPosition = 'CenterScreen'

$okButton = New-Object System.Windows.Forms.Button
$okButton.Location = New-Object System.Drawing.Point(75,120)
$okButton.Size = New-Object System.Drawing.Size(75,23)
$okButton.Text = 'OK'
$okButton.DialogResult = [System.Windows.Forms.DialogResult]::OK
$form.AcceptButton = $okButton
$form.Controls.Add($okButton)

$cancelButton = New-Object System.Windows.Forms.Button
$cancelButton.Location = New-Object System.Drawing.Point(150,120)
$cancelButton.Size = New-Object System.Drawing.Size(75,23)
$cancelButton.Text = 'Cancel'
$cancelButton.DialogResult = [System.Windows.Forms.DialogResult]::Cancel
$form.CancelButton = $cancelButton
$form.Controls.Add($cancelButton)

$label = New-Object System.Windows.Forms.Label
$label.Location = New-Object System.Drawing.Point(10,20)
$label.Size = New-Object System.Drawing.Size(280,20)
$label.Text = 'Please enter the information in the space below:'
$form.Controls.Add($label)

$textBox = New-Object System.Windows.Forms.TextBox
$textBox.Location = New-Object System.Drawing.Point(10,40)
$textBox.Size = New-Object System.Drawing.Size(260,20)
$form.Controls.Add($textBox)

$form.Topmost = $true

$form.Add_Shown({$textBox.Select()})
$result = $form.ShowDialog()

if ($result -eq [System.Windows.Forms.DialogResult]::OK)
{
	$x = $textBox.Text
	$x
}

//------------------------------------------------------------------------------------------------
https://en.cppreference.com/w/cpp/utility/format/format

#include <format>
#include <iostream>
#include <string>
#include <string_view>


template <typename... Args>
std::string dyna_print(std::string_view rt_fmt_str, Args&&... args) {
	return std::vformat(rt_fmt_str, std::make_format_args(args...));
}

int main() {
	std::cout << std::format("Hello {}!\n", "world");

	std::string fmt;
	for (int i{}; i != 3; ++i) {
		fmt += "{} "; // constructs the formatting string
		std::cout << fmt << " : ";
		std::cout << dyna_print(fmt, "alpha", 'Z', 3.14, "unused");
		std::cout << '\n';
	}
}



*/

namespace scrat
{

	class SCRAT_API object
	{
		_decl_objname

	public:

		using list = std::vector<object*>;
		using iterator = object::list::iterator;

		object();
		object(object* parent_);

		virtual ~object();

		template<typename T> T* parent()
		{
			if (_parent) return dynamic_cast<T*>(_parent);
            return nullptr;
		}

		object::iterator query(object* obj);

	protected:

		object::list _children;

		void detach(object* obj);
		result<> add_child(object* obj);

		object* _parent = nullptr;


	};

}
