#/******************************************************************************************
# *   Copyright (C) /.../2024 by Serge Lussier                                             *
# *   lussier.serge@gmail.com                                                              *
# *                                                                                        *
# *   Unless otherwise specified, all code in this project is written                      *
# *   by the author (Serge Lussier)                                                        *
# *   ----------------------------------------------------------------------------------   *
# *   Copyrights from authors other than Serge Lussier also apply here                     *
# *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
# *   ----------------------------------------------------------------------------------   *
# *****************************************************************************************/
#

add_executable(Scrat.Test.App Tests.cxx)

target_link_libraries(Scrat.Test.App scrat)
