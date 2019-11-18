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
#include "pch.hpp"
#include <ssphh_unicornfish.hpp>


namespace Uf
{
	void DoBroker(const char * endpoint, Unicornfish * context)
	{
		if (!context) return;
		Uf::Broker broker;
		context->SetUIMessage(Unicornfish::NodeType::Broker, "started");
		bool result = broker.Create(endpoint);
		while (result && !context->IsStopped()) {
			result = broker.RunLoop();
			std::ostringstream ostr;
			ostr << "Workers Total/Waiting/Requests: ";
			ostr << broker.GetNumWorkers() << "/";
			ostr << broker.GetNumWaitingWorkers() << "/";
			ostr << broker.GetNumRequests();
			context->SetUIMessage(Unicornfish::NodeType::Broker, ostr.str());
		}
		if (!result) {
			HFLOGINFO("%s(): broker: error!");
		}
		broker.Delete();
		HFLOGINFO("broker: okay, quitting");
		context->SetUIMessage(Unicornfish::NodeType::Broker, "stopped");
	}
}