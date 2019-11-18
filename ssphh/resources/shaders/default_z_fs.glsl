// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#version 400

// Uniforms

uniform vec3 Kd;

// Inputs

in vec4 VS_Position;

// Outputs

out vec4 FS_outputColor;

// Main Function

void main(void)
{
	vec4 dP1 = dFdx(VS_Position);
	vec4 dP2 = dFdy(VS_Position);
	vec3 N = normalize(cross(dP1.xyz, dP2.xyz));

	FS_outputColor = vec4(N*0.5+0.5, 1.0);
}