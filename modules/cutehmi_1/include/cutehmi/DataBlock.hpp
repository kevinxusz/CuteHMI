#ifndef H_MODULES_CUTEHMI__1_INCLUDE_CUTEHMI_DATABLOCK_HPP
#define H_MODULES_CUTEHMI__1_INCLUDE_CUTEHMI_DATABLOCK_HPP

#include "internal/common.hpp"
#include "NonCopyable.hpp"

#include <memory>

namespace cutehmi {

/**
 * Data block. Data block is meant to act as a container, which serves its
 * purpose through inheritance. It provides interface, which gives derived
 * classes the ability to be chained together. Each data block object can have
 * a data block child appended to it. This functionality is similar to singly
 * linked list, however the objective is to make data block extensible, rather
 * than manipulative. Instances of this class are non-copyable, but movable.
 *
 * @principle{cutehmi::DataBlock-determined_destruction_order}
 * Child objects are destroyed in the reverse order as they were appended.
 */
class CUTEHMI_API DataBlock:
	public NonCopyable
{
	template <typename BLOCK>
	struct Iterator;

	public:
		typedef Iterator<DataBlock> iterator;	///< Iterator.
		typedef Iterator<const DataBlock> const_iterator;	///< Const iterator.

		/**
		 * Default constructor.
		 */
		DataBlock();

		virtual ~DataBlock() = default;

		/**
		 * Child data block.
		 * @return child data block or @p nullptr.
		 */
		DataBlock * child() const;

		/**
		 * Append data block. Data block is appended to the last child in the chain.
		 * @param block data block to be appended.
		 *
		 * @note This class is not intended to be used with long data chains. The amount of time used by append operation is linearly
		 * proportional to the size of the chain; O(n) in terms of algorithmic complexity.
		 */
		void append(std::unique_ptr<DataBlock> block);

		/**
		 * Count blocks in the chain.
		 * @return number of blocks appended to the current block.
		 */
		int count() const;

		/**
		 * Get iterator pointing to first element in a sequence of data blocks.
		 * @return iterator pointing to first element.
		 */
		iterator begin();

		/**
		 * Get iterator pointing to first element in a sequence of data blocks
		 * (const version)s.
		 * @return iterator pointing to first element.
		 */
		const_iterator begin() const;

		/**
		 * Get iterator pointing to past-the-end element in a sequence of data
		 * blocks.
		 * @return iterator referring to past-the-end element.
		 */
		iterator end();

		/**
		 * Get iterator pointing to past-the-end element in a sequence of data
		 * blocks (const version).
		 * @return iterator referring to past-the-end element.
		 */
		const_iterator end() const;

	protected:
		DataBlock(DataBlock && other) = default;

		DataBlock & operator =(DataBlock && other) = default;

	private:
		template <typename BLOCK>
		struct Iterator :
			public std::iterator<std::forward_iterator_tag, BLOCK, std::ptrdiff_t>
		{
			typedef std::iterator<std::forward_iterator_tag, BLOCK, std::ptrdiff_t> Parent;

			typedef typename Parent::value_type value_type;
			typedef typename Parent::reference reference;
			typedef typename Parent::difference_type difference_type;
			typedef typename Parent::pointer pointer;
			typedef typename Parent::iterator_category iterator_category;

			// std::forward_iterator_tag
			Iterator();

			/**
			 * Constructor.
			 * @param block data block.
			 */
			Iterator(BLOCK * block);

			// std::forward_iterator_tag
			reference operator *() const;

			// std::forward_iterator_tag
			pointer operator ->() const;

			// std::forward_iterator_tag
			Iterator & operator ++();

			// std::forward_iterator_tag
			Iterator operator ++(int);

			// std::forward_iterator_tag
			bool operator ==(const Iterator & other) const;

			// std::forward_iterator_tag
			bool operator !=(const Iterator & other) const;

			private:
				BLOCK * m_me;
		};

		struct Members
		{
			//<principle_ref id="cutehmi::DataBlock-determined_destruction_order">
			std::unique_ptr<DataBlock> child;
			//</principle_ref>
		};

		MPtr<Members> m;
};

template <typename BLOCK>
DataBlock::Iterator<BLOCK>::Iterator():
	m_me(nullptr)
{
}

template <typename BLOCK>
DataBlock::Iterator<BLOCK>::Iterator(BLOCK * block):
	m_me(block)
{
}

template <typename BLOCK>
typename DataBlock::Iterator<BLOCK>::reference DataBlock::Iterator<BLOCK>::operator *() const
{
	Q_ASSERT(m_me != nullptr);

	return *m_me;
}

template <typename BLOCK>
typename DataBlock::Iterator<BLOCK>::pointer DataBlock::Iterator<BLOCK>::operator ->() const
{
	Q_ASSERT(m_me != nullptr);

	return m_me;
}

template <typename BLOCK>
DataBlock::Iterator<BLOCK> & DataBlock::Iterator<BLOCK>::operator ++()
{
	Q_ASSERT(m_me != nullptr);

	m_me = m_me->child();

	return *this;
}

template <typename BLOCK>
DataBlock::Iterator<BLOCK> DataBlock::Iterator<BLOCK>::operator ++(int)
{
	Iterator<BLOCK> copy(*this);
	++(*this);
	return copy;
}

template <typename BLOCK>
bool DataBlock::Iterator<BLOCK>::operator ==(const Iterator & other) const
{
	return other.m_me == m_me;
}

template <typename BLOCK>
bool DataBlock::Iterator<BLOCK>::operator !=(const Iterator & other) const
{
	return other.m_me != m_me;
}

}

#endif

//(c)MP: Copyright © 2018, Michal Policht. All rights reserved.
//(c)MP: This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
