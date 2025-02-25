/*
 * Auto-generated by ./gen-verstrs.pl on Fri Jan 31 15:20:09 2020
 */

#include <assert.h>
#include <string.h>

#include "lsquic.h"

struct lsquic_engine;

static const char *const versions_to_string[ 1 << N_LSQVER ] = {
    [0] = "",
    [(1<<LSQVER_039)] = "39",
    [(1<<LSQVER_043)] = "43",
    [(1<<LSQVER_039)|(1<<LSQVER_043)] = "39,43",
    [(1<<LSQVER_046)] = "46",
    [(1<<LSQVER_039)|(1<<LSQVER_046)] = "39,46",
    [(1<<LSQVER_043)|(1<<LSQVER_046)] = "43,46",
    [(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)] = "39,43,46",
    [(1<<LSQVER_050)] = "50",
    [(1<<LSQVER_039)|(1<<LSQVER_050)] = "39,50",
    [(1<<LSQVER_043)|(1<<LSQVER_050)] = "43,50",
    [(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_050)] = "39,43,50",
    [(1<<LSQVER_046)|(1<<LSQVER_050)] = "46,50",
    [(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_050)] = "39,46,50",
    [(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)] = "43,46,50",
    [(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)] = "39,43,46,50",
};


const char *
lsquic_get_alt_svc_versions (unsigned versions)
{
    /* Limit to versions in versions_to_string: */
    versions &= ((1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050));
    return versions_to_string[ versions ];
}

static const struct {
    unsigned    versions;
    const char *h3_alpns[7];
} vers_2_h3_alnps[] = {
    { 0, { NULL }},
    {(1<<LSQVER_039), { "h3-Q039", NULL }},
    {(1<<LSQVER_043), { "h3-Q043", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043), { "h3-Q039", "h3-Q043", NULL }},
    {(1<<LSQVER_046), { "h3-Q046", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046), { "h3-Q039", "h3-Q046", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046), { "h3-Q043", "h3-Q046", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046), { "h3-Q039", "h3-Q043", "h3-Q046", NULL }},
    {(1<<LSQVER_050), { "h3-Q050", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_050), { "h3-Q039", "h3-Q050", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_050), { "h3-Q043", "h3-Q050", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_050), { "h3-Q039", "h3-Q043", "h3-Q050", NULL }},
    {(1<<LSQVER_046)|(1<<LSQVER_050), { "h3-Q046", "h3-Q050", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_050), { "h3-Q039", "h3-Q046", "h3-Q050", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050), { "h3-Q043", "h3-Q046", "h3-Q050", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050), { "h3-Q039", "h3-Q043", "h3-Q046", "h3-Q050", NULL }},
    {(1<<LSQVER_ID23), { "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_ID23), { "h3-Q039", "h3-23", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_ID23), { "h3-Q043", "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_ID23), { "h3-Q039", "h3-Q043", "h3-23", NULL }},
    {(1<<LSQVER_046)|(1<<LSQVER_ID23), { "h3-Q046", "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_ID23), { "h3-Q039", "h3-Q046", "h3-23", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_ID23), { "h3-Q043", "h3-Q046", "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_ID23), { "h3-Q039", "h3-Q043", "h3-Q046", "h3-23", NULL }},
    {(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q039", "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q043", "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q039", "h3-Q043", "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q046", "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q039", "h3-Q046", "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q043", "h3-Q046", "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23), { "h3-Q039", "h3-Q043", "h3-Q046", "h3-Q050", "h3-23", NULL }},
    {(1<<LSQVER_ID24), { "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_ID24), { "h3-Q039", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_ID24), { "h3-Q043", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-24", NULL }},
    {(1<<LSQVER_046)|(1<<LSQVER_ID24), { "h3-Q046", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q046", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_ID24), { "h3-Q043", "h3-Q046", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-Q046", "h3-24", NULL }},
    {(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q043", "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q046", "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q046", "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q043", "h3-Q046", "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-Q046", "h3-Q050", "h3-24", NULL }},
    {(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q043", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_046)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q046", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q046", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q043", "h3-Q046", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-Q046", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q050", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q050", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q043", "h3-Q050", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-Q050", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q046", "h3-Q050", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q046", "h3-Q050", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q043", "h3-Q046", "h3-Q050", "h3-23", "h3-24", NULL }},
    {(1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24), { "h3-Q039", "h3-Q043", "h3-Q046", "h3-Q050", "h3-23", "h3-24", NULL }},
};

const char *const *
lsquic_get_h3_alpns (unsigned versions)
{
    unsigned i;

    versions &= ((1<<LSQVER_039)|(1<<LSQVER_043)|(1<<LSQVER_046)|(1<<LSQVER_050)|(1<<LSQVER_ID23)|(1<<LSQVER_ID24));

    for (i = 0; i < sizeof(vers_2_h3_alnps) / sizeof(vers_2_h3_alnps[0]); ++i)
        if (versions == vers_2_h3_alnps[i].versions)
            return vers_2_h3_alnps[i].h3_alpns;

    assert(0);
    return vers_2_h3_alnps[0].h3_alpns;
}

enum lsquic_version
lsquic_alpn2ver (const char *alpn, size_t len)
{
    static const struct el {
        size_t len;
        char alpn[10];
        enum lsquic_version version;
    } map[] = {
        {sizeof("h3-Q039")-1,"h3-Q039", LSQVER_039},
        {sizeof("h3-Q043")-1,"h3-Q043", LSQVER_043},
        {sizeof("h3-Q046")-1,"h3-Q046", LSQVER_046},
        {sizeof("h3-Q050")-1,"h3-Q050", LSQVER_050},
        {sizeof("h3-23")-1,"h3-23", LSQVER_ID23},
        {sizeof("h3-24")-1,"h3-24", LSQVER_ID24},
    };
    const struct el *el;

    if (alpn)
        for (el = map; el < map + sizeof(map) / sizeof(map[0]); ++el)
            if (el->len == len && 0 == strncmp(el->alpn, alpn, len))
                return el->version;

    return -1;
}
