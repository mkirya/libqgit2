/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef LIBQGIT2_INDEX_H
#define LIBQGIT2_INDEX_H

#include "libqgit2_export.h"

#include <git2/index.h>

namespace LibQGit2
{
    class Repository;
    class LIBQGIT2_INDEX_EXPORT Index
    {
        public:
            /**
             * Create a new Git index object as a memory representation
             * of the Git index file in 'index_path', without a repository
             * to back it.
             *
             * Since there is no ODB behind this index, any Index methods
             * which rely on the ODB (e.g. index_add) will fail with the
             * GIT_EBAREINDEX error code.
             *
             * @param index_path the path to the index file in disk
             * @return 0 on success; error code otherwise
             */
            Index( const QString& indexPath, QObject* parent = 0 );

            /**
             * Open the Index inside the git repository pointed
             * by 'repo'.
             *
             * @param repo the git repo which owns the index
             * @param index_path the path to the index file in disk
             * @return 0 on success; error code otherwise
             */
            Index(Repository* repository, QObject* parent = 0 );

            Index( const Index& other );

            /**
             * Destruct an existing index object.
             */
            ~Index();

        public:

            /**
             * Clear the contents (all the entries) of an index object.
             * This clears the index object in memory; changes must be manually
             * written to disk for them to take effect.
             */
            void clear();

            /**
            * Update the contents of an existing index object in memory
            * by reading from the hard disk.
            *
            * @return 0 on success, otherwise an error code
            */
            int read() const;

            /**
            * Write an existing index object from memory back to disk
            * using an atomic file lock.
            *
            * @return 0 on success, otherwise an error code
            */
            int write();

            /**
             * Find the first index of any entires which point to given
             * path in the Git index.
             *
             * @param path path to search
             * @return an index >= 0 if found, -1 otherwise
             */
            int find(const QString& path);

            /**
             * Add or update an index entry from a file in disk.
             *
             * @param path filename to add
             * @param stage stage for the entry
             * @return 0 on success, otherwise an error code
             */
            int add(const QString& path, int stage);

            /**
             * Remove an entry from the index
             *
             * @param position position of the entry to remove
             * @return 0 on success, otherwise an error code
             */
            int remove(int position);

            /**
             * Insert an entry into the index.
             * A full copy (including the 'path' string) of the given
             * 'source_entry' will be inserted on the index; if the index
             * already contains an entry for the same path, the entry
             * will be updated.
             *
             * @param source_entry new entry object
             * @return 0 on success, otherwise an error code
             */
            int insert(const git_index_entry *source_entry);

            /**
             * Get a pointer to one of the entries in the index
             *
             * This entry can be modified, and the changes will be written
             * back to disk on the next write() call.
             *
             * @param n the position of the entry
             * @return a pointer to the entry; NULL if out of bounds
             */
            git_index_entry* get(int n) const;

            /**
             * Get the count of entries currently in the index
             *
             * @return integer of count of current entries
             */
            unsigned int entryCount() const;

            git_index* data() const;
            const git_index* constData() const;

        private:
            git_index *m_index;
    };
}

#endif // LIBQGIT2_INDEX_H
