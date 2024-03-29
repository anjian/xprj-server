/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

/*
 * MT safe
 */

//#include "config.h"

#include <interface/baseos/MemoryIf.h>
//#include <common/inc/glist.h>

//#include <string.h>  /* memset */

#include <common/inc/MyStdLibrary.h>
#include <common/inc/ghash.h>

//#include "gstrfuncs.h"
//#include "gatomic.h"
//#include "gtestutils.h"

#define g_free(ptr)     xfree(ptr)

/**
 * SECTION:hash_tables
 * @title: Hash Tables
 * @short_description: associations between keys and values so that
 *                     given a key the value can be found quickly
 *
 * A #GHashTable provides associations between keys and values which is
 * optimized so that given a key, the associated value can be found
 * very quickly.
 *
 * Note that neither keys nor values are copied when inserted into the
 * #GHashTable, so they must exist for the lifetime of the #GHashTable.
 * This means that the use of static strings is OK, but temporary
 * strings (i.e. those created in buffers and those returned by GTK+
 * widgets) should be copied with g_strdup() before being inserted.
 *
 * If keys or values are dynamically allocated, you must be careful to
 * ensure that they are freed when they are removed from the
 * #GHashTable, and also when they are overwritten by new insertions
 * into the #GHashTable. It is also not advisable to mix static strings
 * and dynamically-allocated strings in a #GHashTable, because it then
 * becomes difficult to determine whether the string should be freed.
 *
 * To create a #GHashTable, use g_hash_table_new().
 *
 * To insert a key and value into a #GHashTable, use
 * g_hash_table_insert().
 *
 * To lookup a value corresponding to a given key, use
 * g_hash_table_lookup() and g_hash_table_lookup_extended().
 *
 * g_hash_table_lookup_extended() can also be used to simply
 * check if a key is present in the hash table.
 *
 * To remove a key and value, use g_hash_table_remove().
 *
 * To call a function for each key and value pair use
 * g_hash_table_foreach() or use a iterator to iterate over the
 * key/value pairs in the hash table, see #GHashTableIter.
 *
 * To destroy a #GHashTable use g_hash_table_destroy().
 *
 * <example>
 * <title>Using a GHashTable as a set</title>
 * <para>
 * A common use-case for hash tables is to store information about
 * a set of keys, without associating any particular value with each
 * key. GHashTable optimizes one way of doing so: If you store only
 * key-value pairs where key == value, then GHashTable does not
 * allocate memory to store the values, which can be a considerable
 * space saving, if your set is large.
 * </para>
 * <programlisting>
 * GHashTable *
 * set_new (GHashFunc      hash_func,
 *          GEqualFunc     equal_func,
 *          GDestroyNotify destroy)
 * {
 *   return g_hash_table_new_full (hash_func, equal_func, destroy, NULL);
 * }
 *
 * void
 * set_insert (GHashTable *set,
 *             gpointer    element)
 * {
 *   g_hash_table_insert (set, element, element);
 * }
 *
 * gboolean
 * set_contains (GHashTable *set,
 *               gpointer    element)
 * {
 *   return g_hash_table_lookup_extended (set, element, NULL, NULL);
 * }
 *
 * gboolean
 * set_remove (GHashTable *set,
 *             gpointer    element)
 * {
 *   return g_hash_table_remove (set, element);
 * }
 * </programlisting>
 * </example>
 */

/**
 * GHashTable:
 *
 * The #GHashTable struct is an opaque data structure to represent a
 * <link linkend="glib-Hash-Tables">Hash Table</link>. It should only be
 * accessed via the following functions.
 **/

/**
 * GHashFunc:
 * @key: a key.
 * @Returns: the hash value corresponding to the key.
 *
 * Specifies the type of the hash function which is passed to
 * g_hash_table_new() when a #GHashTable is created.
 *
 * The function is passed a key and should return a #guint hash value.
 * The functions g_direct_hash(), g_int_hash() and g_str_hash() provide
 * hash functions which can be used when the key is a #gpointer, #gint,
 * and #gchar* respectively.
 *
 * <!-- FIXME: Need more here. --> The hash values should be evenly
 * distributed over a fairly large range? The modulus is taken with the
 * hash table size (a prime number) to find the 'bucket' to place each
 * key into. The function should also be very fast, since it is called
 * for each key lookup.
 **/

/**
 * GHFunc:
 * @key: a key.
 * @value: the value corresponding to the key.
 * @user_data: user data passed to g_hash_table_foreach().
 *
 * Specifies the type of the function passed to g_hash_table_foreach().
 * It is called with each key/value pair, together with the @user_data
 * parameter which is passed to g_hash_table_foreach().
 **/

/**
 * GHRFunc:
 * @key: a key.
 * @value: the value associated with the key.
 * @user_data: user data passed to g_hash_table_remove().
 * @Returns: %TRUE if the key/value pair should be removed from the
 *           #GHashTable.
 *
 * Specifies the type of the function passed to
 * g_hash_table_foreach_remove(). It is called with each key/value
 * pair, together with the @user_data parameter passed to
 * g_hash_table_foreach_remove(). It should return %TRUE if the
 * key/value pair should be removed from the #GHashTable.
 **/

/**
 * GEqualFunc:
 * @a: a value.
 * @b: a value to compare with.
 * @Returns: %TRUE if @a = @b; %FALSE otherwise.
 *
 * Specifies the type of a function used to test two values for
 * equality. The function should return %TRUE if both values are equal
 * and %FALSE otherwise.
 **/

/**
 * GHashTableIter:
 *
 * A GHashTableIter structure represents an iterator that can be used
 * to iterate over the elements of a #GHashTable. GHashTableIter
 * structures are typically allocated on the stack and then initialized
 * with g_hash_table_iter_init().
 **/

#define HASH_TABLE_MIN_SHIFT 3  /* 1 << 3 == 8 buckets */

#define UNUSED_HASH_VALUE 0
#define TOMBSTONE_HASH_VALUE 1
#define HASH_IS_UNUSED(h_) ((h_) == UNUSED_HASH_VALUE)
#define HASH_IS_TOMBSTONE(h_) ((h_) == TOMBSTONE_HASH_VALUE)
#define HASH_IS_REAL(h_) ((h_) >= 2)

struct _GHashTable
{
  gint             size;
  gint             mod;
  guint            mask;
  gint             nnodes;
  gint             noccupied;  /* nnodes + tombstones */

  gpointer        *keys;
  guint           *hashes;
  gpointer        *values;

  GHashFunc        hash_func;
  GEqualFunc       key_equal_func;
  gint             ref_count;
#ifndef G_DISABLE_ASSERT
  /*
   * Tracks the structure of the hash table, not its contents: is only
   * incremented when a node is added or removed (is not incremented
   * when the key or data of a node is modified).
   */
  int              version;
#endif
  GDestroyNotify   key_destroy_func;
  GDestroyNotify   value_destroy_func;
};

typedef struct
{
  GHashTable  *hash_table;
  gpointer     dummy1;
  gpointer     dummy2;
  int          position;
  gboolean     dummy3;
  int          version;
} RealIter;

/* Each table size has an associated prime modulo (the first prime
 * lower than the table size) used to find the initial bucket. Probing
 * then works modulo 2^n. The prime modulo is necessary to get a
 * good distribution with poor hash functions. */
static const gint prime_mod [] =
{
  1,          /* For 1 << 0 */
  2,
  3,
  7,
  13,
  31,
  61,
  127,
  251,
  509,
  1021,
  2039,
  4093,
  8191,
  16381,
  32749,
  65521,      /* For 1 << 16 */
  131071,
  262139,
  524287,
  1048573,
  2097143,
  4194301,
  8388593,
  16777213,
  33554393,
  67108859,
  134217689,
  268435399,
  536870909,
  1073741789,
  2147483647  /* For 1 << 31 */
};

static void
g_hash_table_set_shift (GHashTable *hash_table, gint shift)
{
  gint i;
  guint mask = 0;

  hash_table->size = 1 << shift;
  hash_table->mod  = prime_mod [shift];

  for (i = 0; i < shift; i++)
    {
      mask <<= 1;
      mask |= 1;
    }

  hash_table->mask = mask;
}

static gint
g_hash_table_find_closest_shift (gint n)
{
  gint i;

  for (i = 0; n; i++)
    n >>= 1;

  return i;
}

static void
g_hash_table_set_shift_from_size (GHashTable *hash_table, gint size)
{
  gint shift;

  shift = g_hash_table_find_closest_shift (size);
  shift = MAX (shift, HASH_TABLE_MIN_SHIFT);

  g_hash_table_set_shift (hash_table, shift);
}

/*
 * g_hash_table_lookup_node:
 * @hash_table: our #GHashTable
 * @key: the key to lookup against
 * @hash_return: key hash return location
 * Return value: index of the described node
 *
 * Performs a lookup in the hash table, preserving extra information
 * usually needed for insertion.
 *
 * This function first computes the hash value of the key using the
 * user's hash function.
 *
 * If an entry in the table matching @key is found then this function
 * returns the index of that entry in the table, and if not, the
 * index of an unused node (empty or tombstone) where the key can be
 * inserted.
 *
 * The computed hash value is returned in the variable pointed to
 * by @hash_return. This is to save insertions from having to compute
 * the hash record again for the new record.
 */
static inline guint
g_hash_table_lookup_node (GHashTable    *hash_table,
                          gconstpointer  key,
                          guint         *hash_return)
{
  guint node_index;
  guint node_hash;
  guint hash_value;
  guint first_tombstone = 0;
  gboolean have_tombstone = FALSE;
  guint step = 0;

  hash_value = hash_table->hash_func (key);
  if (G_UNLIKELY (!HASH_IS_REAL (hash_value)))
    hash_value = 2;

  *hash_return = hash_value;

  node_index = hash_value % hash_table->mod;
  node_hash = hash_table->hashes[node_index];

  while (!HASH_IS_UNUSED (node_hash))
    {
      /* We first check if our full hash values
       * are equal so we can avoid calling the full-blown
       * key equality function in most cases.
       */
      if (node_hash == hash_value)
        {
          gpointer node_key = hash_table->keys[node_index];

          if (hash_table->key_equal_func)
            {
              if (hash_table->key_equal_func (node_key, key))
                return node_index;
            }
          else if (node_key == key)
            {
              return node_index;
            }
        }
      else if (HASH_IS_TOMBSTONE (node_hash) && !have_tombstone)
        {
          first_tombstone = node_index;
          have_tombstone = TRUE;
        }

      step++;
      node_index += step;
      node_index &= hash_table->mask;
      node_hash = hash_table->hashes[node_index];
    }

  if (have_tombstone)
    return first_tombstone;

  return node_index;
}

/*
 * g_hash_table_remove_node:
 * @hash_table: our #GHashTable
 * @node: pointer to node to remove
 * @notify: %TRUE if the destroy notify handlers are to be called
 *
 * Removes a node from the hash table and updates the node count.
 * The node is replaced by a tombstone. No table resize is performed.
 *
 * If @notify is %TRUE then the destroy notify functions are called
 * for the key and value of the hash node.
 */
static void
g_hash_table_remove_node (GHashTable   *hash_table,
                          int           i,
                          gboolean      notify)
{
  gpointer key;
  gpointer value;

  key = hash_table->keys[i];
  value = hash_table->values[i];

  /* Erect tombstone */
  hash_table->hashes[i] = TOMBSTONE_HASH_VALUE;

  /* Be GC friendly */
  hash_table->keys[i] = NULL;
  hash_table->values[i] = NULL;

  hash_table->nnodes--;

  if (notify && hash_table->key_destroy_func)
    hash_table->key_destroy_func (key);

  if (notify && hash_table->value_destroy_func)
    hash_table->value_destroy_func (value);

}

/*
 * g_hash_table_remove_all_nodes:
 * @hash_table: our #GHashTable
 * @notify: %TRUE if the destroy notify handlers are to be called
 *
 * Removes all nodes from the table.  Since this may be a precursor to
 * freeing the table entirely, no resize is performed.
 *
 * If @notify is %TRUE then the destroy notify functions are called
 * for the key and value of the hash node.
 */
static void
g_hash_table_remove_all_nodes (GHashTable *hash_table,
                               gboolean    notify)
{
  int i;
  gpointer key;
  gpointer value;

  hash_table->nnodes = 0;
  hash_table->noccupied = 0;

  if (!notify ||
      (hash_table->key_destroy_func == NULL &&
       hash_table->value_destroy_func == NULL))
    {
      xmemset (hash_table->hashes, 0, hash_table->size * sizeof (guint));
      xmemset (hash_table->keys, 0, hash_table->size * sizeof (gpointer));
      xmemset (hash_table->values, 0, hash_table->size * sizeof (gpointer));

      return;
    }

  for (i = 0; i < hash_table->size; i++)
    {
      if (HASH_IS_REAL (hash_table->hashes[i]))
        {
          key = hash_table->keys[i];
          value = hash_table->values[i];

          hash_table->hashes[i] = UNUSED_HASH_VALUE;
          hash_table->keys[i] = NULL;
          hash_table->values[i] = NULL;

          if (hash_table->key_destroy_func != NULL)
            hash_table->key_destroy_func (key);

          if (hash_table->value_destroy_func != NULL)
            hash_table->value_destroy_func (value);
        }
      else if (HASH_IS_TOMBSTONE (hash_table->hashes[i]))
        {
          hash_table->hashes[i] = UNUSED_HASH_VALUE;
        }
    }
}

/*
 * g_hash_table_resize:
 * @hash_table: our #GHashTable
 *
 * Resizes the hash table to the optimal size based on the number of
 * nodes currently held.  If you call this function then a resize will
 * occur, even if one does not need to occur.  Use
 * g_hash_table_maybe_resize() instead.
 *
 * This function may "resize" the hash table to its current size, with
 * the side effect of cleaning up tombstones and otherwise optimizing
 * the probe sequences.
 */
static void
g_hash_table_resize (GHashTable *hash_table)
{
  gpointer *new_keys;
  gpointer *new_values;
  guint *new_hashes;
  gint old_size;
  gint i;

  old_size = hash_table->size;
  g_hash_table_set_shift_from_size (hash_table, hash_table->nnodes * 2);

#if 0
  new_keys = g_new0 (gpointer, hash_table->size);
  if (hash_table->keys == hash_table->values)
    new_values = new_keys;
  else
    new_values = g_new0 (gpointer, hash_table->size);
  new_hashes = g_new0 (guint, hash_table->size);
#else
  new_keys = (gpointer*)xmalloc(sizeof(gpointer) * hash_table->size);
  xmemset(new_keys, 0, sizeof(gpointer) * hash_table->size);

  if (hash_table->keys == hash_table->values)
  {
    new_values = new_keys;
  }
  else
  {
    new_values = (gpointer*)xmalloc (sizeof(gpointer) * hash_table->size);
    xmemset(new_values, 0, sizeof(gpointer) * hash_table->size);
  }

  new_hashes = (guint*)xmalloc(sizeof(guint) * hash_table->size);
  xmemset(new_hashes, 0, sizeof(guint) * hash_table->size);
#endif

  for (i = 0; i < old_size; i++)
    {
      guint node_hash = hash_table->hashes[i];
      guint hash_val;
      guint step = 0;

      if (!HASH_IS_REAL (node_hash))
        continue;

      hash_val = node_hash % hash_table->mod;

      while (!HASH_IS_UNUSED (new_hashes[hash_val]))
        {
          step++;
          hash_val += step;
          hash_val &= hash_table->mask;
        }

      new_hashes[hash_val] = hash_table->hashes[i];
      new_keys[hash_val] = hash_table->keys[i];
      new_values[hash_val] = hash_table->values[i];
    }

  if (hash_table->keys != hash_table->values)
  {
    g_free (hash_table->values);
  }

  g_free (hash_table->keys);
  g_free (hash_table->hashes);

  hash_table->keys = new_keys;
  hash_table->values = new_values;
  hash_table->hashes = new_hashes;

  hash_table->noccupied = hash_table->nnodes;
}

/*
 * g_hash_table_maybe_resize:
 * @hash_table: our #GHashTable
 *
 * Resizes the hash table, if needed.
 *
 * Essentially, calls g_hash_table_resize() if the table has strayed
 * too far from its ideal size for its number of nodes.
 */
static inline void
g_hash_table_maybe_resize (GHashTable *hash_table)
{
  gint noccupied = hash_table->noccupied;
  gint size = hash_table->size;

  if ((size > hash_table->nnodes * 4 && size > 1 << HASH_TABLE_MIN_SHIFT) ||
      (size <= noccupied + (noccupied / 16)))
    g_hash_table_resize (hash_table);
}

/**
 * g_hash_table_new:
 * @hash_func: a function to create a hash value from a key.
 *   Hash values are used to determine where keys are stored within the
 *   #GHashTable data structure. The g_direct_hash(), g_int_hash(),
 *   g_int64_hash(), g_double_hash() and g_str_hash() functions are provided
 *   for some common types of keys.
 *   If hash_func is %NULL, g_direct_hash() is used.
 * @key_equal_func: a function to check two keys for equality.  This is
 *   used when looking up keys in the #GHashTable.  The g_direct_equal(),
 *   g_int_equal(), g_int64_equal(), g_double_equal() and g_str_equal()
 *   functions are provided for the most common types of keys.
 *   If @key_equal_func is %NULL, keys are compared directly in a similar
 *   fashion to g_direct_equal(), but without the overhead of a function call.
 *
 * Creates a new #GHashTable with a reference count of 1.
 *
 * Return value: a new #GHashTable.
 **/
GHashTable*
g_hash_table_new (GHashFunc    hash_func,
                  GEqualFunc   key_equal_func)
{
  return g_hash_table_new_full (hash_func, key_equal_func, NULL, NULL);
}


/**
 * g_hash_table_new_full:
 * @hash_func: a function to create a hash value from a key.
 * @key_equal_func: a function to check two keys for equality.
 * @key_destroy_func: a function to free the memory allocated for the key
 *   used when removing the entry from the #GHashTable or %NULL if you
 *   don't want to supply such a function.
 * @value_destroy_func: a function to free the memory allocated for the
 *   value used when removing the entry from the #GHashTable or %NULL if
 *   you don't want to supply such a function.
 *
 * Creates a new #GHashTable like g_hash_table_new() with a reference count
 * of 1 and allows to specify functions to free the memory allocated for the
 * key and value that get called when removing the entry from the #GHashTable.
 *
 * Return value: a new #GHashTable.
 **/
GHashTable*
g_hash_table_new_full (GHashFunc       hash_func,
                       GEqualFunc      key_equal_func,
                       GDestroyNotify  key_destroy_func,
                       GDestroyNotify  value_destroy_func)
{
  GHashTable *hash_table;

#if 0
  hash_table = g_slice_new (GHashTable);
#else
  hash_table = xmalloc (sizeof(GHashTable));
#endif

  g_hash_table_set_shift (hash_table, HASH_TABLE_MIN_SHIFT);
  hash_table->nnodes             = 0;
  hash_table->noccupied          = 0;
  hash_table->hash_func          = hash_func ? hash_func : g_direct_hash;
  hash_table->key_equal_func     = key_equal_func;
  hash_table->ref_count          = 1;
#ifndef G_DISABLE_ASSERT
  hash_table->version            = 0;
#endif
  hash_table->key_destroy_func   = key_destroy_func;
  hash_table->value_destroy_func = value_destroy_func;
#if 0
  hash_table->keys               = g_new0 (gpointer, hash_table->size);
#else
  hash_table->keys               = (gpointer)xmalloc(sizeof(gpointer) * hash_table->size);
  xmemset(hash_table->keys, 0, sizeof(gpointer) * hash_table->size);
#endif
  hash_table->values             = hash_table->keys;
#if 0
  hash_table->hashes             = g_new0 (guint, hash_table->size);
#else
  hash_table->hashes             = (gpointer)xmalloc(sizeof(guint) * hash_table->size);
  xmemset(hash_table->hashes, 0, sizeof(guint) * hash_table->size);
#endif

  return hash_table;
}

/**
 * g_hash_table_iter_init:
 * @iter: an uninitialized #GHashTableIter.
 * @hash_table: a #GHashTable.
 *
 * Initializes a key/value pair iterator and associates it with
 * @hash_table. Modifying the hash table after calling this function
 * invalidates the returned iterator.
 * |[
 * GHashTableIter iter;
 * gpointer key, value;
 *
 * g_hash_table_iter_init (&iter, hash_table);
 * while (g_hash_table_iter_next (&iter, &key, &value)) 
 *   {
 *     /&ast; do something with key and value &ast;/
 *   }
 * ]|
 *
 * Since: 2.16
 **/
void
g_hash_table_iter_init (GHashTableIter *iter,
                        GHashTable     *hash_table)
{
  RealIter *ri = (RealIter *) iter;

  g_return_if_fail (iter != NULL);
  g_return_if_fail (hash_table != NULL);

  ri->hash_table = hash_table;
  ri->position = -1;
#ifndef G_DISABLE_ASSERT
  ri->version = hash_table->version;
#endif
}

/**
 * g_hash_table_iter_next:
 * @iter: an initialized #GHashTableIter.
 * @key: a location to store the key, or %NULL.
 * @value: a location to store the value, or %NULL.
 *
 * Advances @iter and retrieves the key and/or value that are now
 * pointed to as a result of this advancement. If %FALSE is returned,
 * @key and @value are not set, and the iterator becomes invalid.
 *
 * Return value: %FALSE if the end of the #GHashTable has been reached.
 *
 * Since: 2.16
 **/
gboolean
g_hash_table_iter_next (GHashTableIter *iter,
                        gpointer       *key,
                        gpointer       *value)
{
  RealIter *ri = (RealIter *) iter;
  gint position;

  g_return_val_if_fail (iter != NULL, FALSE);
#ifndef G_DISABLE_ASSERT
  g_return_val_if_fail (ri->version == ri->hash_table->version, FALSE);
#endif
  g_return_val_if_fail (ri->position < ri->hash_table->size, FALSE);

  position = ri->position;

  do
    {
      position++;
      if (position >= ri->hash_table->size)
        {
          ri->position = position;
          return FALSE;
        }
    }
  while (!HASH_IS_REAL (ri->hash_table->hashes[position]));

  if (key != NULL)
    *key = ri->hash_table->keys[position];
  if (value != NULL)
    *value = ri->hash_table->values[position];

  ri->position = position;
  return TRUE;
}

/**
 * g_hash_table_iter_get_hash_table:
 * @iter: an initialized #GHashTableIter.
 *
 * Returns the #GHashTable associated with @iter.
 *
 * Return value: the #GHashTable associated with @iter.
 *
 * Since: 2.16
 **/
GHashTable *
g_hash_table_iter_get_hash_table (GHashTableIter *iter)
{
  g_return_val_if_fail (iter != NULL, NULL);

  return ((RealIter *) iter)->hash_table;
}

static void
iter_remove_or_steal (RealIter *ri, gboolean notify)
{
  g_return_if_fail (ri != NULL);
#ifndef G_DISABLE_ASSERT
  g_return_if_fail (ri->version == ri->hash_table->version);
#endif
  g_return_if_fail (ri->position >= 0);
  g_return_if_fail (ri->position < ri->hash_table->size);

  g_hash_table_remove_node (ri->hash_table, ri->position, notify);

#ifndef G_DISABLE_ASSERT
  ri->version++;
  ri->hash_table->version++;
#endif
}

/**
 * g_hash_table_iter_remove:
 * @iter: an initialized #GHashTableIter.
 *
 * Removes the key/value pair currently pointed to by the iterator
 * from its associated #GHashTable. Can only be called after
 * g_hash_table_iter_next() returned %TRUE, and cannot be called more
 * than once for the same key/value pair.
 *
 * If the #GHashTable was created using g_hash_table_new_full(), the
 * key and value are freed using the supplied destroy functions, otherwise
 * you have to make sure that any dynamically allocated values are freed 
 * yourself.
 *
 * Since: 2.16
 **/
void
g_hash_table_iter_remove (GHashTableIter *iter)
{
  iter_remove_or_steal ((RealIter *) iter, TRUE);
}

/*
 * g_hash_table_insert_node:
 * @hash_table: our #GHashTable
 * @node_index: pointer to node to insert/replace
 * @key_hash: key hash
 * @key: key to replace with, or %NULL
 * @value: value to replace with
 * @keep_new_key: whether to replace the key in the node with @key
 * @reusing_key: whether @key was taken out of the existing node
 *
 * Inserts a value at @node_index in the hash table and updates it.
 *
 * If @key has been taken out of the existing node (ie it is not
 * passed in via a g_hash_table_insert/replace) call, then @reusing_key
 * should be %TRUE.
 */
static void
g_hash_table_insert_node (GHashTable *hash_table,
                          guint       node_index,
                          guint       key_hash,
                          gpointer    key,
                          gpointer    value,
                          gboolean    keep_new_key,
                          gboolean    reusing_key)
{
  guint old_hash;
  gpointer old_key;
  gpointer old_value;

  if (G_UNLIKELY (hash_table->keys == hash_table->values && key != value))
  {
#if 0
    hash_table->values = g_memdup (hash_table->keys, sizeof (gpointer) * hash_table->size);
#else
    hash_table->values = (gpointer)xmalloc(sizeof(gpointer) * hash_table->size);
    xmemcpy(hash_table->values, hash_table->keys, sizeof(gpointer) * hash_table->size);
#endif
  }

  old_hash = hash_table->hashes[node_index];
  old_key = hash_table->keys[node_index];
  old_value = hash_table->values[node_index];

  if (HASH_IS_REAL (old_hash))
    {
      if (keep_new_key)
        hash_table->keys[node_index] = key;
      hash_table->values[node_index] = value;
    }
  else
    {
      hash_table->keys[node_index] = key;
      hash_table->values[node_index] = value;
      hash_table->hashes[node_index] = key_hash;

      hash_table->nnodes++;

      if (HASH_IS_UNUSED (old_hash))
        {
          /* We replaced an empty node, and not a tombstone */
          hash_table->noccupied++;
          g_hash_table_maybe_resize (hash_table);
        }

#ifndef G_DISABLE_ASSERT
      hash_table->version++;
#endif
    }

  if (HASH_IS_REAL (old_hash))
    {
      if (hash_table->key_destroy_func && !reusing_key)
        hash_table->key_destroy_func (keep_new_key ? old_key : key);
      if (hash_table->value_destroy_func)
        hash_table->value_destroy_func (old_value);
    }
}

/**
 * g_hash_table_iter_replace:
 * @iter: an initialized #GHashTableIter.
 * @value: the value to replace with
 *
 * Replaces the value currently pointed to by the iterator
 * from its associated #GHashTable. Can only be called after
 * g_hash_table_iter_next() returned %TRUE.
 *
 * If you supplied a @value_destroy_func when creating the #GHashTable,
 * the old value is freed using that function.
 *
 * Since: 2.29.9
 **/
void
g_hash_table_iter_replace (GHashTableIter *iter,
                           gpointer        value)
{
  RealIter *ri;
  guint node_hash;
  gpointer key;

  ri = (RealIter *) iter;

  g_return_if_fail (ri != NULL);
#ifndef G_DISABLE_ASSERT
  g_return_if_fail (ri->version == ri->hash_table->version);
#endif
  g_return_if_fail (ri->position >= 0);
  g_return_if_fail (ri->position < ri->hash_table->size);

  node_hash = ri->hash_table->hashes[ri->position];
  key = ri->hash_table->keys[ri->position];

  g_hash_table_insert_node (ri->hash_table, ri->position, node_hash, key, value, TRUE, TRUE);

#ifndef G_DISABLE_ASSERT
  ri->version++;
  ri->hash_table->version++;
#endif
}

/**
 * g_hash_table_iter_steal:
 * @iter: an initialized #GHashTableIter.
 *
 * Removes the key/value pair currently pointed to by the iterator
 * from its associated #GHashTable, without calling the key and value
 * destroy functions. Can only be called after
 * g_hash_table_iter_next() returned %TRUE, and cannot be called more
 * than once for the same key/value pair.
 *
 * Since: 2.16
 **/
void
g_hash_table_iter_steal (GHashTableIter *iter)
{
  iter_remove_or_steal ((RealIter *) iter, FALSE);
}


/**
 * g_hash_table_ref:
 * @hash_table: a valid #GHashTable.
 *
 * Atomically increments the reference count of @hash_table by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Return value: the passed in #GHashTable.
 *
 * Since: 2.10
 **/
GHashTable*
g_hash_table_ref (GHashTable *hash_table)
{
  g_return_val_if_fail (hash_table != NULL, NULL);

#if 0
  g_atomic_int_inc (&hash_table->ref_count);
#else
  hash_table->ref_count ++;
#endif

  return hash_table;
}

/**
 * g_hash_table_unref:
 * @hash_table: a valid #GHashTable.
 *
 * Atomically decrements the reference count of @hash_table by one.
 * If the reference count drops to 0, all keys and values will be
 * destroyed, and all memory allocated by the hash table is released.
 * This function is MT-safe and may be called from any thread.
 *
 * Since: 2.10
 **/
void
g_hash_table_unref (GHashTable *hash_table)
{
  g_return_if_fail (hash_table != NULL);

#if 0
  if (g_atomic_int_dec_and_test (&hash_table->ref_count))
#else
  hash_table->ref_count--;
  if (hash_table->ref_count <= 0)
#endif
    {
      g_hash_table_remove_all_nodes (hash_table, TRUE);
      if (hash_table->keys != hash_table->values)
        g_free (hash_table->values);
      g_free (hash_table->keys);
      g_free (hash_table->hashes);

#if 0
      g_slice_free (GHashTable, hash_table);
#else
      xfree(hash_table);
#endif
    }
}

/**
 * g_hash_table_destroy:
 * @hash_table: a #GHashTable.
 *
 * Destroys all keys and values in the #GHashTable and decrements its
 * reference count by 1. If keys and/or values are dynamically allocated,
 * you should either free them first or create the #GHashTable with destroy
 * notifiers using g_hash_table_new_full(). In the latter case the destroy
 * functions you supplied will be called on all keys and values during the
 * destruction phase.
 **/
void
g_hash_table_destroy (GHashTable *hash_table)
{
  g_return_if_fail (hash_table != NULL);

  g_hash_table_remove_all (hash_table);
  g_hash_table_unref (hash_table);
}

/**
 * g_hash_table_lookup:
 * @hash_table: a #GHashTable.
 * @key: the key to look up.
 *
 * Looks up a key in a #GHashTable. Note that this function cannot
 * distinguish between a key that is not present and one which is present
 * and has the value %NULL. If you need this distinction, use
 * g_hash_table_lookup_extended().
 *
 * Return value: the associated value, or %NULL if the key is not found.
 **/
gpointer
g_hash_table_lookup (GHashTable   *hash_table,
                     gconstpointer key)
{
  guint node_index;
  guint node_hash;

  g_return_val_if_fail (hash_table != NULL, NULL);

  node_index = g_hash_table_lookup_node (hash_table, key, &node_hash);

  return HASH_IS_REAL (hash_table->hashes[node_index])
    ? hash_table->values[node_index]
    : NULL;
}

/**
 * g_hash_table_lookup_extended:
 * @hash_table: a #GHashTable
 * @lookup_key: the key to look up
 * @orig_key: return location for the original key, or %NULL
 * @value: return location for the value associated with the key, or %NULL
 *
 * Looks up a key in the #GHashTable, returning the original key and the
 * associated value and a #gboolean which is %TRUE if the key was found. This
 * is useful if you need to free the memory allocated for the original key,
 * for example before calling g_hash_table_remove().
 *
 * You can actually pass %NULL for @lookup_key to test
 * whether the %NULL key exists, provided the hash and equal functions
 * of @hash_table are %NULL-safe.
 *
 * Return value: %TRUE if the key was found in the #GHashTable.
 **/
gboolean
g_hash_table_lookup_extended (GHashTable    *hash_table,
                              gconstpointer  lookup_key,
                              gpointer      *orig_key,
                              gpointer      *value)
{
  guint node_index;
  guint node_hash;

  g_return_val_if_fail (hash_table != NULL, FALSE);

  node_index = g_hash_table_lookup_node (hash_table, lookup_key, &node_hash);

  if (!HASH_IS_REAL (hash_table->hashes[node_index]))
    return FALSE;

  if (orig_key)
    *orig_key = hash_table->keys[node_index];

  if (value)
    *value = hash_table->values[node_index];

  return TRUE;
}

/*
 * g_hash_table_insert_internal:
 * @hash_table: our #GHashTable
 * @key: the key to insert
 * @value: the value to insert
 * @keep_new_key: if %TRUE and this key already exists in the table
 *   then call the destroy notify function on the old key.  If %FALSE
 *   then call the destroy notify function on the new key.
 *
 * Implements the common logic for the g_hash_table_insert() and
 * g_hash_table_replace() functions.
 *
 * Do a lookup of @key.  If it is found, replace it with the new
 * @value (and perhaps the new @key).  If it is not found, create a
 * new node.
 */
static void
g_hash_table_insert_internal (GHashTable *hash_table,
                              gpointer    key,
                              gpointer    value,
                              gboolean    keep_new_key)
{
  guint key_hash;
  guint node_index;

  g_return_if_fail (hash_table != NULL);

  node_index = g_hash_table_lookup_node (hash_table, key, &key_hash);

  g_hash_table_insert_node (hash_table, node_index, key_hash, key, value, keep_new_key, FALSE);
}

/**
 * g_hash_table_insert:
 * @hash_table: a #GHashTable.
 * @key: a key to insert.
 * @value: the value to associate with the key.
 *
 * Inserts a new key and value into a #GHashTable.
 *
 * If the key already exists in the #GHashTable its current value is replaced
 * with the new value. If you supplied a @value_destroy_func when creating the
 * #GHashTable, the old value is freed using that function. If you supplied
 * a @key_destroy_func when creating the #GHashTable, the passed key is freed
 * using that function.
 **/
void
g_hash_table_insert (GHashTable *hash_table,
                     gpointer    key,
                     gpointer    value)
{
  g_hash_table_insert_internal (hash_table, key, value, FALSE);
}

/**
 * g_hash_table_replace:
 * @hash_table: a #GHashTable.
 * @key: a key to insert.
 * @value: the value to associate with the key.
 *
 * Inserts a new key and value into a #GHashTable similar to
 * g_hash_table_insert(). The difference is that if the key already exists
 * in the #GHashTable, it gets replaced by the new key. If you supplied a
 * @value_destroy_func when creating the #GHashTable, the old value is freed
 * using that function. If you supplied a @key_destroy_func when creating the
 * #GHashTable, the old key is freed using that function.
 **/
void
g_hash_table_replace (GHashTable *hash_table,
                      gpointer    key,
                      gpointer    value)
{
  g_hash_table_insert_internal (hash_table, key, value, TRUE);
}

/*
 * g_hash_table_remove_internal:
 * @hash_table: our #GHashTable
 * @key: the key to remove
 * @notify: %TRUE if the destroy notify handlers are to be called
 * Return value: %TRUE if a node was found and removed, else %FALSE
 *
 * Implements the common logic for the g_hash_table_remove() and
 * g_hash_table_steal() functions.
 *
 * Do a lookup of @key and remove it if it is found, calling the
 * destroy notify handlers only if @notify is %TRUE.
 */
static gboolean
g_hash_table_remove_internal (GHashTable    *hash_table,
                              gconstpointer  key,
                              gboolean       notify)
{
  guint node_index;
  guint node_hash;

  g_return_val_if_fail (hash_table != NULL, FALSE);

  node_index = g_hash_table_lookup_node (hash_table, key, &node_hash);

  if (!HASH_IS_REAL (hash_table->hashes[node_index]))
    return FALSE;

  g_hash_table_remove_node (hash_table, node_index, notify);
  g_hash_table_maybe_resize (hash_table);

#ifndef G_DISABLE_ASSERT
  hash_table->version++;
#endif

  return TRUE;
}

/**
 * g_hash_table_remove:
 * @hash_table: a #GHashTable.
 * @key: the key to remove.
 *
 * Removes a key and its associated value from a #GHashTable.
 *
 * If the #GHashTable was created using g_hash_table_new_full(), the
 * key and value are freed using the supplied destroy functions, otherwise
 * you have to make sure that any dynamically allocated values are freed
 * yourself.
 *
 * Return value: %TRUE if the key was found and removed from the #GHashTable.
 **/
gboolean
g_hash_table_remove (GHashTable    *hash_table,
                     gconstpointer  key)
{
  return g_hash_table_remove_internal (hash_table, key, TRUE);
}

/**
 * g_hash_table_steal:
 * @hash_table: a #GHashTable.
 * @key: the key to remove.
 *
 * Removes a key and its associated value from a #GHashTable without
 * calling the key and value destroy functions.
 *
 * Return value: %TRUE if the key was found and removed from the #GHashTable.
 **/
gboolean
g_hash_table_steal (GHashTable    *hash_table,
                    gconstpointer  key)
{
  return g_hash_table_remove_internal (hash_table, key, FALSE);
}

/**
 * g_hash_table_remove_all:
 * @hash_table: a #GHashTable
 *
 * Removes all keys and their associated values from a #GHashTable.
 *
 * If the #GHashTable was created using g_hash_table_new_full(), the keys
 * and values are freed using the supplied destroy functions, otherwise you
 * have to make sure that any dynamically allocated values are freed
 * yourself.
 *
 * Since: 2.12
 **/
void
g_hash_table_remove_all (GHashTable *hash_table)
{
  g_return_if_fail (hash_table != NULL);

#ifndef G_DISABLE_ASSERT
  if (hash_table->nnodes != 0)
    hash_table->version++;
#endif

  g_hash_table_remove_all_nodes (hash_table, TRUE);
  g_hash_table_maybe_resize (hash_table);
}

/**
 * g_hash_table_steal_all:
 * @hash_table: a #GHashTable.
 *
 * Removes all keys and their associated values from a #GHashTable
 * without calling the key and value destroy functions.
 *
 * Since: 2.12
 **/
void
g_hash_table_steal_all (GHashTable *hash_table)
{
  g_return_if_fail (hash_table != NULL);

#ifndef G_DISABLE_ASSERT
  if (hash_table->nnodes != 0)
    hash_table->version++;
#endif

  g_hash_table_remove_all_nodes (hash_table, FALSE);
  g_hash_table_maybe_resize (hash_table);
}

/*
 * g_hash_table_foreach_remove_or_steal:
 * @hash_table: our #GHashTable
 * @func: the user's callback function
 * @user_data: data for @func
 * @notify: %TRUE if the destroy notify handlers are to be called
 *
 * Implements the common logic for g_hash_table_foreach_remove() and
 * g_hash_table_foreach_steal().
 *
 * Iterates over every node in the table, calling @func with the key
 * and value of the node (and @user_data).  If @func returns %TRUE the
 * node is removed from the table.
 *
 * If @notify is true then the destroy notify handlers will be called
 * for each removed node.
 */
static guint
g_hash_table_foreach_remove_or_steal (GHashTable *hash_table,
                                      GHRFunc     func,
                                      gpointer    user_data,
                                      gboolean    notify)
{
  guint deleted = 0;
  gint i;
#ifndef G_DISABLE_ASSERT
  gint version = hash_table->version;
#endif

  for (i = 0; i < hash_table->size; i++)
    {
      guint node_hash = hash_table->hashes[i];
      gpointer node_key = hash_table->keys[i];
      gpointer node_value = hash_table->values[i];

      if (HASH_IS_REAL (node_hash) &&
          (* func) (node_key, node_value, user_data))
        {
          g_hash_table_remove_node (hash_table, i, notify);
          deleted++;
        }

#ifndef G_DISABLE_ASSERT
      g_return_val_if_fail (version == hash_table->version, 0);
#endif
    }

  g_hash_table_maybe_resize (hash_table);

#ifndef G_DISABLE_ASSERT
  if (deleted > 0)
    hash_table->version++;
#endif

  return deleted;
}

/**
 * g_hash_table_foreach_remove:
 * @hash_table: a #GHashTable.
 * @func: the function to call for each key/value pair.
 * @user_data: user data to pass to the function.
 *
 * Calls the given function for each key/value pair in the #GHashTable.
 * If the function returns %TRUE, then the key/value pair is removed from the
 * #GHashTable. If you supplied key or value destroy functions when creating
 * the #GHashTable, they are used to free the memory allocated for the removed
 * keys and values.
 *
 * See #GHashTableIter for an alternative way to loop over the 
 * key/value pairs in the hash table.
 *
 * Return value: the number of key/value pairs removed.
 **/
guint
g_hash_table_foreach_remove (GHashTable *hash_table,
                             GHRFunc     func,
                             gpointer    user_data)
{
  g_return_val_if_fail (hash_table != NULL, 0);
  g_return_val_if_fail (func != NULL, 0);

  return g_hash_table_foreach_remove_or_steal (hash_table, func, user_data, TRUE);
}

/**
 * g_hash_table_foreach_steal:
 * @hash_table: a #GHashTable.
 * @func: the function to call for each key/value pair.
 * @user_data: user data to pass to the function.
 *
 * Calls the given function for each key/value pair in the #GHashTable.
 * If the function returns %TRUE, then the key/value pair is removed from the
 * #GHashTable, but no key or value destroy functions are called.
 *
 * See #GHashTableIter for an alternative way to loop over the
 * key/value pairs in the hash table.
 *
 * Return value: the number of key/value pairs removed.
 **/
guint
g_hash_table_foreach_steal (GHashTable *hash_table,
                            GHRFunc     func,
                            gpointer    user_data)
{
  g_return_val_if_fail (hash_table != NULL, 0);
  g_return_val_if_fail (func != NULL, 0);

  return g_hash_table_foreach_remove_or_steal (hash_table, func, user_data, FALSE);
}

/**
 * g_hash_table_foreach:
 * @hash_table: a #GHashTable.
 * @func: the function to call for each key/value pair.
 * @user_data: user data to pass to the function.
 *
 * Calls the given function for each of the key/value pairs in the
 * #GHashTable.  The function is passed the key and value of each
 * pair, and the given @user_data parameter.  The hash table may not
 * be modified while iterating over it (you can't add/remove
 * items). To remove all items matching a predicate, use
 * g_hash_table_foreach_remove().
 *
 * See g_hash_table_find() for performance caveats for linear
 * order searches in contrast to g_hash_table_lookup().
 **/
void
g_hash_table_foreach (GHashTable *hash_table,
                      GHFunc      func,
                      gpointer    user_data)
{
  gint i;
#ifndef G_DISABLE_ASSERT
  gint version = hash_table->version;
#endif

  g_return_if_fail (hash_table != NULL);
  g_return_if_fail (func != NULL);

  for (i = 0; i < hash_table->size; i++)
    {
      guint node_hash = hash_table->hashes[i];
      gpointer node_key = hash_table->keys[i];
      gpointer node_value = hash_table->values[i];

      if (HASH_IS_REAL (node_hash))
        (* func) (node_key, node_value, user_data);

#ifndef G_DISABLE_ASSERT
      g_return_if_fail (version == hash_table->version);
#endif
    }
}

/**
 * g_hash_table_find:
 * @hash_table: a #GHashTable.
 * @predicate:  function to test the key/value pairs for a certain property.
 * @user_data:  user data to pass to the function.
 *
 * Calls the given function for key/value pairs in the #GHashTable until
 * @predicate returns %TRUE.  The function is passed the key and value of
 * each pair, and the given @user_data parameter. The hash table may not
 * be modified while iterating over it (you can't add/remove items).
 *
 * Note, that hash tables are really only optimized for forward lookups,
 * i.e. g_hash_table_lookup().
 * So code that frequently issues g_hash_table_find() or
 * g_hash_table_foreach() (e.g. in the order of once per every entry in a
 * hash table) should probably be reworked to use additional or different
 * data structures for reverse lookups (keep in mind that an O(n) find/foreach
 * operation issued for all n values in a hash table ends up needing O(n*n)
 * operations).
 *
 * Return value: The value of the first key/value pair is returned,
 *     for which @predicate evaluates to %TRUE. If no pair with the
 *     requested property is found, %NULL is returned.
 *
 * Since: 2.4
 **/
gpointer
g_hash_table_find (GHashTable *hash_table,
                   GHRFunc     predicate,
                   gpointer    user_data)
{
  gint i;
#ifndef G_DISABLE_ASSERT
  gint version = hash_table->version;
#endif
  gboolean match;

  g_return_val_if_fail (hash_table != NULL, NULL);
  g_return_val_if_fail (predicate != NULL, NULL);

  match = FALSE;

  for (i = 0; i < hash_table->size; i++)
    {
      guint node_hash = hash_table->hashes[i];
      gpointer node_key = hash_table->keys[i];
      gpointer node_value = hash_table->values[i];

      if (HASH_IS_REAL (node_hash))
        match = predicate (node_key, node_value, user_data);

#ifndef G_DISABLE_ASSERT
      g_return_val_if_fail (version == hash_table->version, NULL);
#endif

      if (match)
        return node_value;
    }

  return NULL;
}

/**
 * g_hash_table_size:
 * @hash_table: a #GHashTable.
 *
 * Returns the number of elements contained in the #GHashTable.
 *
 * Return value: the number of key/value pairs in the #GHashTable.
 **/
guint
g_hash_table_size (GHashTable *hash_table)
{
  g_return_val_if_fail (hash_table != NULL, 0);

  return hash_table->nnodes;
}

/**
 * g_hash_table_get_keys:
 * @hash_table: a #GHashTable
 *
 * Retrieves every key inside @hash_table. The returned data is valid
 * until @hash_table is modified.
 *
 * Return value: a #GList containing all the keys inside the hash
 *   table. The content of the list is owned by the hash table and
 *   should not be modified or freed. Use g_list_free() when done
 *   using the list.
 *
 * Since: 2.14
 */
GList *
g_hash_table_get_keys (GHashTable *hash_table)
{
  gint i;
  GList *retval;

  g_return_val_if_fail (hash_table != NULL, NULL);

  retval = NULL;
  for (i = 0; i < hash_table->size; i++)
    {
      if (HASH_IS_REAL (hash_table->hashes[i]))
        retval = g_list_prepend (retval, hash_table->keys[i]);
    }

  return retval;
}

/**
 * g_hash_table_get_values:
 * @hash_table: a #GHashTable
 *
 * Retrieves every value inside @hash_table. The returned data is
 * valid until @hash_table is modified.
 *
 * Return value: a #GList containing all the values inside the hash
 *   table. The content of the list is owned by the hash table and
 *   should not be modified or freed. Use g_list_free() when done
 *   using the list.
 *
 * Since: 2.14
 */
GList *
g_hash_table_get_values (GHashTable *hash_table)
{
  gint i;
  GList *retval;

  g_return_val_if_fail (hash_table != NULL, NULL);

  retval = NULL;
  for (i = 0; i < hash_table->size; i++)
    {
      if (HASH_IS_REAL (hash_table->hashes[i]))
        retval = g_list_prepend (retval, hash_table->values[i]);
    }

  return retval;
}
