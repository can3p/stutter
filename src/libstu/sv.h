#ifndef SV_DEFINED
#define SV_DEFINED

#include <stdio.h>

#include "gc.h"

#define SV_CONS_REGISTERS 2
#define SV_CAR_REG 0
#define SV_CDR_REG 1

#define IS_NIL(sv)   ((sv) && (sv)->type == SV_NIL ? 1 : ((sv) ? 0 : 1))
#define IS_MACRO(sv) ((sv) && (sv)->type == SV_LAMBDA ? ((sv)->val.ufunc->is_macro) : 0)
#define CAR(sv)      ((sv) && (sv)->type == SV_CONS ? (sv)->val.reg[SV_CAR_REG] : NULL)
#define CDR(sv)      ((sv) && (sv)->type == SV_CONS ? (sv)->val.reg[SV_CDR_REG] : NULL)
#define CADR(sv)     ((sv) ? CAR(CDR((sv))) : NULL)
#define CADDR(sv)    ((sv) ? CAR(CDR(CDR((sv)))) : NULL)

/* Types of stu values. */
enum Sv_type {
    SV_NIL,
    SV_ERR,
    SV_SYM,
    SV_INT,
    SV_FLOAT,
    SV_RATIONAL,
    SV_BOOL,
    SV_STR,
    SV_CONS,
    SV_FUNC,
    SV_LAMBDA,
    SV_SPECIAL,
};

enum Sv_special_type {
    SV_SPECIAL_COMMA,
    SV_SPECIAL_COMMA_SPREAD,
    SV_SPECIAL_BACKQUOTE
};

/* Forward declarations. */
struct Gc;
struct Env;
struct Sv;
struct Stu;

/* Function value definition. */
typedef struct Sv *(*Sv_func)(struct Stu *, struct Env *, struct Sv *);

typedef struct Sv_ufunc {
    struct Env *env;
    struct Sv *formals;
    struct Sv *body;
    short  is_macro;
} Sv_ufunc;

typedef struct Sv_special {
    enum Sv_special_type type;
    struct Sv *body;
} Sv_special;

typedef struct Sv_rational {
    long n;
    long d;
} Sv_rational;

/* Actual value container. */
union Sv_val {
    long i;
    double f;
    char *buf;
    Sv_func func;
    Sv_rational rational;
    struct Sv_special *special;
    struct Sv *reg[SV_CONS_REGISTERS];
    struct Sv_ufunc *ufunc;
};

/* Core stu value. */
typedef struct Sv {
    struct Gc gc;
    unsigned char special;
    enum Sv_type type;
    union Sv_val val;
} Sv;

extern Sv *Sv_new(struct Stu *, enum Sv_type);
extern Sv *Sv_new_int(struct Stu *, long);
extern Sv *Sv_new_float(struct Stu *, double);
extern Sv *Sv_new_rational(struct Stu *, long, long);
extern Sv *Sv_new_bool(struct Stu *, short);
extern Sv *Sv_new_str(struct Stu *, const char *);
extern Sv *Sv_new_sym(struct Stu *, const char *);
extern Sv *Sv_new_err(struct Stu *, const char *);
extern Sv *Sv_new_func(struct Stu *, Sv_func);
extern Sv *Sv_new_lambda(struct Stu *, struct Env *, Sv *, Sv *);
extern Sv *Sv_new_special(struct Stu *, enum Sv_special_type type, Sv *body);

extern void Sv_dump(struct Stu *, Sv *sv, FILE *);
extern void Sv_destroy(Sv **);
extern Sv *Sv_cons(struct Stu *, Sv *, Sv *);
extern Sv *Sv_reverse(struct Stu *, Sv *);
extern Sv *Sv_list(struct Stu *, Sv *);
extern Sv *Sv_copy(struct Stu *, Sv *);
extern Sv *Sv_expand(struct Stu *, Sv *);
extern Sv *Sv_expand_1(struct Stu *, Sv *);

extern Sv *Sv_eval(struct Stu *, struct Env *, Sv *);
extern Sv *Sv_eval_list(struct Stu *, struct Env *, Sv *);
extern Sv *Sv_eval_special(struct Stu *, struct Env *, Sv *);
extern Sv *Sv_eval_special_cons(struct Stu *, struct Env *, Sv *);
extern Sv *Sv_eval_sexp(struct Stu *, struct Env *, Sv *);
extern Sv *Sv_call(struct Stu *, struct Env *, Sv *, Sv *);

#endif
