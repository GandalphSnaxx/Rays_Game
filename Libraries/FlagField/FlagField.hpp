/**
 * @file FlagField.hpp
 * @author Ray Richter
 * @brief Declaration and definition of the FlagField class and class members.
 * @details Operator reference table:
 * LHS type  | OP | RHS type  | RET type  | Description
 * ----------|----|-----------|-----------|-------------
 * Unary Operators
 * OP   | Name                      | Description
 * -----|---------------------------|--------
 * !	| Logical NOT	            | Returns `true` if no flags are set
 * &	| Address-of	            | Not changed
 * ( )	| Cast Operator	            | Casts a byte to a FlagField
 * *	| Pointer dereference	    | Returns the address of the byte arrat
 * +	| Unary Plus prefix	        | Sets every flag
 * +	| Unary Plus postfix	    | ?
 * ++	| Increment 1 prefix	    | Sets the first unset flag
 * ++	| Increment 1 postfix	    | Makes a new FlagField
 * -	| Unary negation prefix	    | Clears every flag
 * -	| Unary negation postfix	| ?
 * --	| Decrement 1 prefix	    | Clears the first set flag
 * --	| Decrement 1 postfix	    | Makes a new FlagField
 * ~	| One's complement	        | Toggles every flag
 * 
 * Binary Operators
 * OP   | Name                      | Description
 * -----|---------------------------|--------
 * ,	| Comma	                    | Combines flags
 * !=	| Inequality	            | Returns `true` if the flag(s) are not set
 * %	| Modulus	                | ?
 * %=	| Modulus assignment	    | ?
 * &    | Bitwise AND	            | Makes a new FlagField with matching flags
 * &&	| Logical AND	            | Returns `true` if matching flags are set
 * &=	| Bitwise AND assignment    | Sets flags
 * ( )	| Function call	            | Returns `true` if every flag is set
 * *	| Multiplication	        | Bool: Creates a new FlagField b ? same : empty
 *      |                           | uint: ? Makes a scaled FlagField ?
 * *=	| Multiplication assignment	| Bool: Clears the FlagField if `false`
 * +	| Addition	                | Makes a new FlagField with matching flags
 * +=	| Addition assignment	    | Sets flags
 * -	| Subtraction	            | Makes a new flagfield without matching flags
 * -=	| Subtraction assignment	| Clears flags
 * ->	| Member selection	        | ? isSet ?
 * ->*	| Pointer-to-member sel	    | ?
 * /	| Division	                | ?
 * /=	| Division assignment	    | ?
 * <	| Less than	                | Compares number of set flags
 * <<	| Left shift	            | ?
 * <<=	| Left shift assignment	    | Sets this FlagField from a bytefield.
 * <=	| Less than or equal to	    | Compares number of set flags
 * =	| Assignment	            | Clear then set
 * ==	| Equality	                | uint: Returns `true` if flag is set
 *      |                           | FlagField: Returns `true` if all flags match
 * >	| Greater than	            | Compares number of set flags
 * >=	| Greater than or equal to	| Compares number of set flags
 * >>	| Right shift	            | ?
 * >>=	| Right shift assignment	| ?
 * [ ]	| Array subscript	        | Binary flag check
 * ^	| Exclusive OR	            | Makes a new FlagField with toggled flags
 * ^=	| Exclusive OR assignment	| Toggles flags
 * |	| Bitwise inclusive OR	    | Makes a new FlagField with OR flags
 * ||	| Logical OR	            | Returns `true` if any flag is matching
 * |=	| Bitwise inclusive OR ass	| Combines flags
 */
#pragma once
#ifndef FLAGFIELD_HPP
#define FLAGFIELD_HPP

// #define FLAGFIELD_DEBUG
// #define FLAGFIELD_NO_VALIDATE

#include <cstdint>
#include <array>
#include <stdexcept>
#include <ostream>

#ifndef FLAGFIELD_DEBUG
#define FF_DEBUG(msg)
#else
#define FF_DEBUG(msg) std::cout << "[FlagField]: " << msg << std::endl
#endif

#ifdef FLAGFIELD_NO_VALIDATE
#define FF_VD(idx, ret)
#elif !defined(FLAGFIELD_DEBUG)
#define FF_VD(idx, ret)                         \
if (idx >= MAX) { return ret; }
#else
#define FF_VD(idx, ret)                         \
if (idx >= MAX) { throw std::out_of_range(      \
    "[FlagField] - ERROR: Index out of range!");\
    return ret;                                 \
}
#endif

/// @brief A class to manage a field of flags.
/// @note FlagFields can be created in reference to enums that define flag indices.
/// @note Example usage: 
/// ```
/// typedef enum Flags {
///     A, B, C, D, E, F, G, H, MAX
/// } Flags;
/// 
/// FlagField<MAX, Flags> example_ff;
///
/// example_ff += B; // Sets flag B
///
/// if (example_ff(B, C)) {} // Checks if flags B and C are set
/// ```
/// @tparam MAX The maximum number of flags to manage. Default = `8`.
/// @tparam E The enum to set as a reference. Default = `size_t`.
template <size_t MAX = 8, class E = size_t>
class FlagField {
    static_assert(std::is_enum<E>::value || std::is_same<E, size_t>::value,
        "[FlagField] - ERROR: FlagField must use an enum or size_t type!");
    // static_assert(std::is_enum<E>::value || std::is_integral<E>::value,
    //     "[FlagField] - ERROR: FlagField must use an enum or integral type!");
    static_assert(MAX > 0, "[FlagField] - ERROR: Number of managed flags must be > 0!");
public:
/// @section Constructors and Deconstructors
    /// @brief Copy constructor. 
    FlagField(const FlagField& other) {
        FF_DEBUG("Creating FlagField from another FlagField both with size: " << size());
        // Initialize flags array to zero
        clear_();
        set_(other);
    }

    /// @brief Explicit single flag constructor. 
    explicit FlagField(const size_t& idx) {
        FF_DEBUG("Creating a FlagField with size: " << size() << ", and flag set at index: " << idx);
        // Initialize flags array to zero
        clear_();
        set_((E)idx);
    }

    /// @brief Explicit constructor from a list of flags.
    template <typename... Fs>
    explicit FlagField(const size_t& idx, const Fs&... idxs) {
        FF_DEBUG("Creating FlagField from a list of flags with size: " << size());
        // Initialize flags array to zero
        clear_();

        *this = FlagField(idxs...);
        set_((E)idx);
    }

    /// @brief Default constructor.
    FlagField() {
        FF_DEBUG("Creating FlagField with default constructor and size: " << size());
        // Initialize flags array to zero
        clear_(); 
    }

    /// @brief Deconstructor.
    ~FlagField() { 
        FF_DEBUG("Deconstructing FlagField with size: " << size());
    }

/// @section Accessors

/// @subsection Set Functions

    /// @brief Sets every flag.
    void set() {
        FF_DEBUG("Setting every flag.");
        set_();
    }

    /// @brief Set a flag at the given index.
    void set(const E& index) {
        FF_VD(index,);
        FF_DEBUG("Set flag at index: " << index);
        set_(index);
    }

    /// @brief Sets flags from another FlagField.
    void set(const FlagField& other) {
        FF_DEBUG("Set flags from another FlagField.");
        set_(other);
    }

    /// @brief Sets a list of flags at the given indices.
    template <typename... O> void set(const E& index, const O&... indices) {
        set(index); set(indices...);
    }

/// @subsection Clear Functions

    /// @brief Clears every flag.
    void clear() {
        FF_DEBUG("Clearing every flag.");
        clear_();
    }

    /// @brief Clears a flag at the given index.
    void clear(const E& index) {
        FF_VD(index,);
        FF_DEBUG("Cleared flag at index: " << index);
        clear_(index);
    }

    /// @brief Clears flags from another FlagField.
    void clear(const FlagField& other) {
        FF_DEBUG("Clearing flags from another FlagField.");
        clear_(other);
    }

    /// @brief Clears flags from a list of flag indices.
    template <typename... O> void clear(const E& index, const O&... indices) {
        clear(index); clear(indices...);
    }

/// @subsection Toggle Functions

    /// @brief Toggles every flag.
    void toggle() {
        FF_DEBUG("Toggling every flag.");
        toggle_();
    }

    /// @brief Toggles a flag at the given index.
    void toggle(const E& index) {
        FF_VD(index,);
        FF_DEBUG("Toggled flag at index: " << index);
        toggle_(index);
    }

    /// @brief Toggles flags from another FlagField.
    void toggle(const FlagField& other) {
        FF_DEBUG("Toggling flags from another FlagField.");
        toggle_(other);
    }

    /// @brief Toggles flags from a list of flag indices.
    template <typename... O> void toggle(const E& index, const O&... indices) {
        toggle(index); toggle(indices...);
    }

/// @subsection Query Functions

    /// @brief Returns `true` if every flag is set.
    bool isSet() const {
        FF_DEBUG("Checking if all " << size() << " flags are set.");
        return isSet_();
    }

    /// @brief Returns `true` if the flag at the given index is set.
    bool isSet(const E& index) const {
        FF_VD(index, false);
        FF_DEBUG("Checking if flag at index " << index << " is set.");
        return isSet_(index);
    }

    /// @brief Returns `true` if the other FlagField's flags are set in this FlagField.
    bool isSet(const FlagField& other) const { 
        FF_DEBUG("Checking if flags match another FlagField's flags.");
        return isSet_(other); 
    }

    /// @brief Returns `true` if every flag at every index is set.
    template <typename... O> bool isSet(const E& index, const O&... indices) const {
        return isSet(index) && isSet(indices...);
    }

    /// @brief Returns `true` if no flags are set.
    bool isNSet() const {
        FF_DEBUG("Checking if no flags are set.");
        return numSetFlags() == 0;
    }

    /// @brief Returns `true` if the flag at the given index is not set.
    bool isNSet(const E& index) const {
        FF_VD(index, false);
        FF_DEBUG("Checking if flag at index " << index << " is not set");
        return !isSet_(index);
    }

    /// @brief Returns `true` if no flags match.
    bool isNSet(const FlagField& other) const {
        FF_DEBUG("Checking if no flags match.");
        const uint8_t mask = (1 << size() % 8) - 1;
        for (size_t i = 0; i < sizeBytes() - (size() % 8 > 0); i++) {
            if ((flags_[i] & other.flags_[i]) != 0) return false;
        }
        return (flags_[sizeBytes() - 1] & 
            other.flags_[sizeBytes() - 1] &
            mask) == 0;
    }

    /// @brief Returns `true` if no flags at the given indices are set.
    template <class... Fs> bool isNSet(const E& idx, const Fs&... idxs) const {
        return isNSet(idx) && isNSet(idxs...);
    }

/// @subsection FlagField State Functions

    /// @brief Gets the number of managed flags.
    constexpr size_t size() const { return MAX; }

    /// @brief Gets the number of bytes managed.
    constexpr size_t sizeBytes() const { return (MAX + 7) / 8; }

    constexpr const char* name() const { return typeid(E).name(); }

    /// @brief Counts the number of set flags.
    size_t numSetFlags() const {
        size_t count = 0;
        uint8_t mask = (1 << size() % 8) - 1;
        if (mask == 0) mask = 0xFF;
        for (size_t i = 0; i < sizeBytes() - 1; i++) { 
            count += countBits_(flags_[i]); 
        }
        return count += countBits_(flags_[sizeBytes() - 1] & mask);
    }

/// @section Operator Overloads

/// @subsection Unary Operators

    /// @brief Returns `true` if no flags are set.
    bool operator!() const { 
        FF_DEBUG("!");
        return !isSet_(); 
    }

    /// @brief Returns a pointer to the flag byte array.
    uint8_t* operator*() { 
        FF_DEBUG("*");
        return &flags_[0]; 
    }

    /// @brief Sets every flag.
    FlagField& operator+() { 
        FF_DEBUG("+");
        set_(); 
        return *this; 
    }

    /// @brief Sets the first unset flag.
    FlagField& operator++() { 
        size_t idx = 0;
        while(idx < size()) {
            // If flag is not set, set it and return
            if (!isSet_(static_cast<E>(idx))) { 
                FF_DEBUG("++ Setting the first unset flag at index: " << idx);
                set_(static_cast<E>(idx)); 
                return *this; 
            }
            idx++;
        }
        // Every flag is set
        return *this;
    }

    /// @brief Sets the first cleared flag.
    FlagField& operator++(int) {
        this->operator++();
        return *this;
    }

    /// @brief Clears every flag.
    FlagField& operator-() { 
        FF_DEBUG("-");
        clear_(); 
        return *this; 
    }

    /// @brief Clears the first set flag.
    FlagField& operator--() {
        size_t idx = 0;
        while(idx < size()) {
            // If flag is set, clear it and return
            if (isSet_(static_cast<E>(idx))) { 
                FF_DEBUG("-- Clearing the first set flag at index: " << idx);
                clear_(static_cast<E>(idx)); 
                return *this; 
            }
            idx++;
        }
        // Every flag is set
        return *this;
    }

    /// @brief Clears the first set flag.
    FlagField& operator--(int) {
        this->operator--();
        return *this;
    }

    /// @brief Toggles every flag.
    FlagField& operator~() { 
        FF_DEBUG("~");
        toggle_(); 
        return *this; 
    }

/// @subsection Binary Operators

    /// @brief Sets flags at the given indices.
    FlagField& operator,(const E& idx) { 
        FF_VD(idx, *this);
        FF_DEBUG(", " << idx);
        set_(idx); 
        return *this; 
    }
    /// @brief Sets flags at the given indices.
    FlagField& operator,(const FlagField& other) { 
        FF_DEBUG(", other");
        set_(other); 
        return *this; 
    }

/// @subsubsection Comparison Operator Functions

    /// @brief Returns `true` if the indexed flag is set.
    bool operator==(const E& idx) const { 
        FF_VD(idx, false);
        FF_DEBUG("== " << idx);
        return isSet_(idx); 
    }
    /// @brief Returns `true` if every flag matches.
    bool operator==(const FlagField& other) const { 
        FF_DEBUG("== other");
        return isSet_(other); 
    }

    /// @brief Returns `true` if the flag at `idx` is not set.
    bool operator!=(const E& idx) const { 
        FF_VD(idx, false);
        FF_DEBUG("!= " << idx);
        return isNSet_(idx); 
    }
    /// @brief Returns `true` if every set flag in other is not set.
    bool operator!=(const FlagField& other) const { 
        FF_DEBUG("!= other");
        return isNSet_(other); 
    }

    /// @brief Compares the number of set flags.
    template <size_t M, class X> 
    bool operator< (const FlagField<M, X>& other) const { 
        FF_DEBUG("< other");
        return numSetFlags() <  other.numSetFlags(); 
    }
    /// @brief Compares the number of set flags.
    template <size_t M, class X> 
    bool operator<=(const FlagField<M, X>& other) const { 
        FF_DEBUG("<= other");
        return numSetFlags() <= other.numSetFlags(); 
    }
    /// @brief Compares the number of set flags.
    template <size_t M, class X> 
    bool operator> (const FlagField<M, X>& other) const { 
        FF_DEBUG("> other");
        return numSetFlags() > other.numSetFlags(); 
    }
    /// @brief Compares the number of set flags.
    template <size_t M, class X> 
    bool operator>=(const FlagField<M, X>& other) const { 
        FF_DEBUG(">= other");
        return numSetFlags() >= other.numSetFlags(); 
    }

/// @subsubsection AND Operator Functions

    /// @brief Returns `true` if matching flags are set.
    bool operator&&(const E& idx) const { 
        FF_VD(idx, false);
        FF_DEBUG("&& " << idx);
        return isSet_(idx); 
    }
    /// @brief Returns `true` if matching flags are set.
    bool operator&&(const FlagField& other) const { 
        FF_DEBUG("&& other");
        return isSet_(other); 
    }

    /// @brief Bitwise AND assignment.
    FlagField& operator&=(const E& idx) {
        FF_VD(idx, *this);
        FF_DEBUG("&= " << idx);
        bool keep = isSet_(idx);
        clear_();
        if (keep) { set_(idx); }
        return *this;
    }
    /// @brief Bitwise AND assignment.
    FlagField& operator&=(const FlagField& other) {
        FF_DEBUG("&= other");
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] &= other.flags_[i];
        }
        return *this;
    }

    /// @brief Makes a new FlagField with matching flags.
    FlagField operator&(const E& idx) const {
        FlagField x;
        FF_VD(idx, x);
        FF_DEBUG("& " << idx);
        x.set_(idx);
        return x;
    }
    /// @brief Makes a new FlagField with matching flags.
    FlagField operator&(const FlagField& other) const {
        FlagField x = *this;
        FF_DEBUG("& other");
        for (size_t i = 0; i < sizeBytes(); i++) {
            x.flags_[i] &= other.flags_[i];
        }
        return x;
    }

/// @subsubsection OR Operator Functions

    /// @brief Returns `true` if any flag matches.
    bool operator||(const E& idx) const { 
        FF_VD(idx, false);
        FF_DEBUG("|| " << idx);
        return isSet_(idx); 
    }
    /// @brief Returns `true` if any flag matches.
    bool operator||(const FlagField& other) const {
        FF_DEBUG("|| other"); 
        uint8_t mask = (1 << size() % 8) - 1;
        if (mask == 0) mask = 0xFF;
        for (size_t i = 0; i < sizeBytes() - 1; i++) {
            if ((flags_[i] & other.flags_[i]) != 0) return true;
        }
        return ((flags_[sizeBytes() - 1] & 
            other.flags_[sizeBytes() - 1]) & 
            mask) != 0;
    }

    /// @brief Sets combined flags.
    FlagField& operator|=(const E& idx) {
        FF_VD(idx, *this);
        FF_DEBUG("|= " << idx);
        set_(idx);
        return *this;
    }
    /// @brief Sets combined flags.
    FlagField& operator|=(const FlagField& other) {
        FF_DEBUG("|= other");
        set_(other);
        return *this;
    }

    /// @brief Makes a new FlagField with combined flags.
    FlagField operator|(const E& idx) const {
        FlagField x = *this;
        FF_VD(idx, x);
        FF_DEBUG("| " << idx);
        x.set_(idx);
        return x;
    }
    /// @brief Makes a new FlagField with combined flags.
    FlagField operator|(const FlagField& other) const {
        FlagField x = *this;
        FF_DEBUG("| other");
        x.set_(other);
        return x;
    }

/// @subsubsection Assignment Operators

    /// @brief Sets only the flag at the given index.
    FlagField& operator=(const E& idx) {
        FF_VD(idx, *this);
        FF_DEBUG("= " << idx);
        clear_();
        set_(idx);
        return *this;
    }
    /// @brief Sets the flags from another FlagField.
    FlagField& operator=(const FlagField& other) {
        FF_DEBUG("= other");
        if (this != &other) {
            clear_();
            set_(other);
        }
        return *this;
    }

    /// @brief Sets this FlagField from a bytefield.
    /// @note Only converts up to 8 flags.
    FlagField& operator<<=(const uint8_t& byte) {
        FF_DEBUG("<<= 1 byte bytefield");
        clear_();
        flags_[0] = byte;
        return *this;
    }

    // Removed for safety
    // /// @brief Sets this FlagField from a bytefield.
    // /// @warning If the byte array is shorter than the number of managed bytes, unexpected errors can occur!
    // FlagField& operator<<=(const uint8_t* bytes) {
    //     FF_DEBUG("<<= bytefield");
    //     for (size_t i = 0; i < sizeBytes(); i++) {
    //         flags_[i] = bytes[i];
    //     }
    //     return *this;
    // }

    /// @brief Sets this FlagField from a bytefield.
    FlagField& operator<<=(const std::array<uint8_t, (MAX + 7) / 8>& bytes) {
        FF_DEBUG("<<= std::array<uint8_t, " << sizeBytes() << ">");
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] = bytes[i];
        }
        return *this;
    }

/// @subsubsection Access Operators

    /// @brief Returns `true` if every flag is set.
    bool operator()(const E& idx) const { 
        FF_VD(idx, false);
        FF_DEBUG("(" << idx << ")");
        return isSet_(idx); 
    }
    /// @brief Returns `true` if every flag is set.
    bool operator()(const FlagField& other) const { 
        FF_DEBUG("(other)");
        return isSet_(other); 
    }
    /// @brief Returns `true` if every flag is set.
    template <typename... Fs> bool operator()(const Fs&... idxs) const { return isSet(idxs...); }

    /// @brief Returns `index` if the indexed flag is set.
    E operator[](const E& idx) const { 
        FF_VD(idx, (E)0);
        FF_DEBUG("[" << idx << "]");
        return E(isSet_(idx) * (size_t)idx); 
    }

/// @subsubsection Arithmatic Operators

    /// @brief Sets the flag at the given index.
    FlagField& operator+=(const E& idx) {
        FF_VD(idx, *this);
        FF_DEBUG("+= " << idx);
        set_(idx); 
        return *this;
    }
    /// @brief Sets the flag at the given index.
    FlagField& operator+=(const FlagField& other) {
        FF_DEBUG("+= other");
        set_(other); 
        return *this;
    }
    /// @brief Sets the flag at the given index.
    FlagField operator+(const E& idx) const {
        FlagField x = *this;
        FF_VD(idx, x);
        FF_DEBUG("+ " << idx);
        x.set_(idx);
        return x;
    }
    /// @brief Sets the flag at the given index.
    FlagField operator+(const FlagField& other) const {
        FlagField x = *this;
        FF_DEBUG("+ other");
        x.set_(other);
        return x;
    }

    /// @brief Clears the flag at the given index.
    FlagField& operator-=(const E& idx) {
        FF_VD(idx, *this);
        FF_DEBUG("-= " << idx);
        clear_(idx); 
        return *this;
    }
    /// @brief Clears the flag at the given index.
    FlagField& operator-=(const FlagField& other) {
        FF_DEBUG("-= other");
        clear_(other); 
        return *this;
    }
    /// @brief Clears the flag at the given index.
    FlagField operator-(const E& idx) const {
        FlagField x = *this;
        FF_VD(idx, x);
        FF_DEBUG("- " << idx);
        x.clear_(idx);
        return x;
    }
    /// @brief Clears the flag at the given index.
    FlagField operator-(const FlagField& other) const {
        FlagField x = *this;
        FF_DEBUG("- other");
        x.clear_(other);
        return x;
    }

    /// @brief Clears every flag if false.
    FlagField& operator*=(const bool& b) {
        FF_DEBUG("*= " << (b ? "true" : "false"));
        if (!b) clear_();
        return *this;
    }
    /// @brief Creates a new empty (if `false`) or identical (if `true`) FlagField.
    FlagField operator*(const bool& b) const {
        FlagField x;
        FF_DEBUG("* " << (b ? "true" : "false"));
        if (b) x.set_(*this);
        return x;
    }

    /// @brief Toggles the flag at the given index.
    FlagField& operator^=(const E& idx) {
        FF_VD(idx, *this);
        FF_DEBUG("^= " << idx);
        toggle_(idx);
        return *this;
    }
    /// @brief Toggles the flags at the given indices.
    FlagField& operator^=(const FlagField& other) {
        FF_DEBUG("^= other");
        toggle_(other);
        return *this;
    }
    /// @brief Toggles the flag at the given index.
    FlagField operator^(const E& idx) const {
        FlagField x = *this;
        FF_VD(idx, x);
        FF_DEBUG("^ " << idx);
        x.toggle_(idx);
        return x;
    }
    /// @brief Toggles the flag at the given index.
    FlagField operator^(const FlagField& idx) const {
        FlagField x = *this;
        FF_DEBUG("^ other");
        x.toggle_(idx);
        return x;
    }

/// @subsection Out Stream Operator Overloads

    friend std::ostream& operator<<(std::ostream& os, const FlagField& ff) {
        os << "FlagField<" << ff.size() << ", " << ff.name() << ">: [";
        for (size_t i = 0; i < ff.size(); i++) {
            if ((i % 4 == 0) && (i != 0) && (i != ff.size() - 1)) os << " ";
            os << (ff.isSet_((E)i) ? "|" : ".");
        }
        return os << "]";
    }
    
/// @section Private Members
private:
    /// @brief An array of flags on the stack.
    uint8_t flags_[(MAX + 7) / 8];

    /// @brief Counts the number of 1s in a byte.
    int countBits_(uint8_t byte) const {
        int count = 0;
        while (byte) { count += byte & 1; byte >>= 1; }
        return count;
    }

    /// @brief Sets every flag.
    void set_() {
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] = 0xFF;
        }
    }

    /// @brief Set a flag at the given index.
    void set_(const E& index) {
        flags_[index / 8] |= (1 << (index % 8));
    }

    /// @brief Sets flags from another FlagField.
    void set_(const FlagField& other) {
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] |= other.flags_[i];
        }
    }

    template <class... Fs> void setList_(const E& idx, const Fs&... idxs) {
        set_(idx); set_(idxs...);
    }

    /// @brief Clears every flag.
    void clear_() {
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] = 0;
        }
    }

    /// @brief Clears a flag at the given index.
    void clear_(const E& index) {
        flags_[index / 8] &= ~(1 << (index % 8));
    }

    /// @brief Clears flags from another FlagField.
    void clear_(const FlagField& other) {
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] &= ~other.flags_[i];
        }
    }

    /// @brief Toggles every flag.
    void toggle_() {
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] ^= 0xFF;
        }
    }

    /// @brief Toggles a flag at the given index.
    void toggle_(const E& index) {
        flags_[index / 8] ^= (1 << (index % 8));
    }

    /// @brief Toggles flags from another FlagField.
    void toggle_(const FlagField& other) {
        for (size_t i = 0; i < sizeBytes(); i++) {
            flags_[i] ^= other.flags_[i];
        }
    }

    /// @brief Checks if every flag is set
    bool isSet_() const {
        // Set a mask for the last byte
        const uint8_t mask = (1 << size() % 8) - 1;
        // Iterate over every fully used byte
        for (size_t i = 0; i < sizeBytes() - 1; i++) {
            if (flags_[i] != 0xFF) return false;
        }
        // Check if every used bit in the last byte is used
        if(mask == 0) return flags_[sizeBytes() - 1] == 0xFF;
        return (flags_[sizeBytes() - 1] & mask) == mask;
    }

    /// @brief Checks if a flag is set
    bool isSet_(const E& idx) const {
        return flags_[idx / 8] & (1 << (idx % 8));
    }

    /// @brief Checks if every set flag is set in this
    bool isSet_(const FlagField& other) const {
        for (size_t i = 0; i < size(); i++) {
            if (other.isSet_((E)i) && !isSet((E)i)) return false;
        }
        return true;
    }
};

/// @section FlagField Related Functions

#endif // FLAGFIELD_HPP