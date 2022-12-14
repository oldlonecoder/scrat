// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SCRAT_SCRATURE_H
#define SCRAT_SCRATURE_H

namespace scrat {

/**
 * @todo write docs
 */
class scrature
{
public:
    /**
     * Default constructor
     */
    scrature();

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    scrature(const scrature& other);

    /**
     * Destructor
     */
    ~scrature();

    /**
     * Assignment operator
     *
     * @param other TODO
     * @return TODO
     */
    scrature& operator=(const scrature& other);

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator==(const scrature& other) const;

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const scrature& other) const;

};

}

#endif // SCRAT_SCRATURE_H
