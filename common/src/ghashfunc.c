////////////////////////////////////////////////////////////////////////////////
// extracted from glib/gstring.c
////////////////////////////////////////////////////////////////////////////////
/* Hash Functions.
 *  */

#include <common/inc/gmacros.h>
#include <common/inc/MyStdLibrary.h>

/**
 * g_str_equal:
 * @v1: a key
 * @v2: a key to compare with @v1
 *
 * Compares two strings for byte-by-byte equality and returns %TRUE
 * if they are equal. It can be passed to g_hash_table_new() as the
 * @key_equal_func parameter, when using strings as keys in a #GHashTable.
 *
 * Note that this function is primarily meant as a hash table comparison
 * function. For a general-purpose, %NULL-safe string comparison function,
 * see g_strcmp0().
 *
 * Returns: %TRUE if the two keys match
 */
gboolean g_str_equal(gconstpointer v1, gconstpointer v2)
{
    const gchar *string1 = v1;
    const gchar *string2 = v2;

    return xstrcmp(string1, string2) == 0;
}

/**
 * g_str_hash:
 * @v: a string key
 *
 * Converts a string to a hash value.
 *
 * This function implements the widely used "djb" hash apparently posted
 * by Daniel Bernstein to comp.lang.c some time ago.  The 32 bit
 * unsigned hash value starts at 5381 and for each byte 'c' in the
 * string, is updated: <literal>hash = hash * 33 + c</literal>.  This
 * function uses the signed value of each byte.
 *
 * It can be passed to g_hash_table_new() as the @hash_func parameter,
 * when using strings as keys in a #GHashTable.
 *
 * Returns: a hash value corresponding to the key
 */
guint g_str_hash (gconstpointer v)
{
    const signed char *p;
    guint32 h = 5381;

    for (p = v; *p != '\0'; p++)
    {
        h = (h << 5) + h + *p;
    }

    return h;
}

////////////////////////////////////////////////////////////////////////////////
// extracted from glib/gutil.c
////////////////////////////////////////////////////////////////////////////////
/**
 * g_int_equal:
 * @v1: a pointer to a #gint key.
 * @v2: a pointer to a #gint key to compare with @v1.
 *
 * Compares the two #gint values being pointed to and returns 
 * %TRUE if they are equal.
 * It can be passed to g_hash_table_new() as the @key_equal_func
 * parameter, when using pointers to integers as keys in a #GHashTable.
 * 
 * Returns: %TRUE if the two keys match.
 */
gboolean g_int_equal (gconstpointer v1, gconstpointer v2)
{
    return *((const gint*) v1) == *((const gint*) v2);
}

/**
 * g_int_hash:
 * @v: a pointer to a #gint key
 *
 * Converts a pointer to a #gint to a hash value.
 * It can be passed to g_hash_table_new() as the @hash_func parameter, 
 * when using pointers to integers values as keys in a #GHashTable.
 *
 * Returns: a hash value corresponding to the key.
 */
guint g_int_hash (gconstpointer v)
{
    return *(const gint*) v;
}

/**
 * g_direct_hash:
 * @v: a #gpointer key
 *
 * Converts a gpointer to a hash value.
 * It can be passed to g_hash_table_new() as the @hash_func parameter, 
 * when using pointers as keys in a #GHashTable.
 *
 * Returns: a hash value corresponding to the key.
 */
guint g_direct_hash (gconstpointer v)
{
#if 0
    return GPOINTER_TO_UINT(v);
#else
    return (guint)(v);
#endif
}

/**
 * g_direct_equal:
 * @v1: a key.
 * @v2: a key to compare with @v1.
 *
 * Compares two #gpointer arguments and returns %TRUE if they are equal.
 * It can be passed to g_hash_table_new() as the @key_equal_func
 * parameter, when using pointers as keys in a #GHashTable.
 * 
 * Returns: %TRUE if the two keys match.
 */
gboolean g_direct_equal (gconstpointer v1, gconstpointer v2)
{
    return v1 == v2;
}

/**
 * g_int64_equal:
 * @v1: a pointer to a #gint64 key.
 * @v2: a pointer to a #gint64 key to compare with @v1.
 *
 * Compares the two #gint64 values being pointed to and returns 
 * %TRUE if they are equal.
 * It can be passed to g_hash_table_new() as the @key_equal_func
 * parameter, when using pointers to 64-bit integers as keys in a #GHashTable.
 * 
 * Returns: %TRUE if the two keys match.
 *
 * Since: 2.22
 */
gboolean g_int64_equal (gconstpointer v1, gconstpointer v2)
{
    return *((const gint64*) v1) == *((const gint64*) v2);
}

/**
 * g_int64_hash:
 * @v: a pointer to a #gint64 key
 *
 * Converts a pointer to a #gint64 to a hash value.
 * It can be passed to g_hash_table_new() as the @hash_func parameter, 
 * when using pointers to 64-bit integers values as keys in a #GHashTable.
 *
 * Returns: a hash value corresponding to the key.
 *
 * Since: 2.22
 */
guint g_int64_hash (gconstpointer v)
{
    return (guint) *(const gint64*) v;
}

/**
 * g_double_equal:
 * @v1: a pointer to a #gdouble key.
 * @v2: a pointer to a #gdouble key to compare with @v1.
 *
 * Compares the two #gdouble values being pointed to and returns 
 * %TRUE if they are equal.
 * It can be passed to g_hash_table_new() as the @key_equal_func
 * parameter, when using pointers to doubles as keys in a #GHashTable.
 * 
 * Returns: %TRUE if the two keys match.
 *
 * Since: 2.22
 */
gboolean g_double_equal (gconstpointer v1, gconstpointer v2)
{
    return *((const gdouble*) v1) == *((const gdouble*) v2);
}

/**
 * g_double_hash:
 * @v: a pointer to a #gdouble key
 *
 * Converts a pointer to a #gdouble to a hash value.
 * It can be passed to g_hash_table_new() as the @hash_func parameter, 
 * when using pointers to doubles as keys in a #GHashTable.
 *
 * Returns: a hash value corresponding to the key.
 *
 * Since: 2.22
 */
guint g_double_hash (gconstpointer v)
{
    return (guint) *(const gdouble*) v;
}

