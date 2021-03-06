#ifndef H_MODULES_CUTEHMI__XML__1_INCLUDE_CUTEHMI_XML_CONVERSIONS_HPP
#define H_MODULES_CUTEHMI__XML__1_INCLUDE_CUTEHMI_XML_CONVERSIONS_HPP

#include "internal/common.hpp"

#include <QString>

namespace cutehmi {
namespace xml {

/**
 * @brief Minimal port number of a random port. By default a number defined by
 * IANA for a dynamic port range is used (http://www.rfc-editor.org/rfc/rfc6335.txt).
 */
CUTEHMI_XML_API extern uint randomPortBegin;

CUTEHMI_XML_API extern uint randomPortEnd;

#ifndef CUTEHMI_XML_RANDOM_PORT_BEGIN
	#define CUTEHMI_XML_RANDOM_PORT_BEGIN 49152 // Minimal port number of a random port. By default a number defined by IANA for a dynamic port range is used (http://www.rfc-editor.org/rfc/rfc6335.txt).
#endif

#ifndef CUTEHMI_XML_RANDOM_PORT_END
	#define CUTEHMI_XML_RANDOM_PORT_END 65535 // Maximal port number of a random port.
#endif

/**
 * Convert string to a port number. Extends standard QString::toUint() function by the
 * ability of picking random port number in a deterministic fashion. If @a string contains
 * pattern 'randPlus([0-9]*)', then randomly selected port number will be used. Number
 * '[0-9]*' indicates a number that should be added to the randomly selected port.
 * @param string string to convert.
 * @param ok set to @p true when conversion was succesful, otherwise set to @p false.
 * @return port number.
 */
CUTEHMI_XML_API uint portFromString(const QString & string, bool & ok);

CUTEHMI_XML_API QString portToString(uint port, bool random);

}
}

#endif

//(c)MP: Copyright © 2018, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
