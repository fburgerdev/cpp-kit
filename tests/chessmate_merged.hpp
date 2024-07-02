#pragma once
// #include "common.hpp" (HPPMERGE)
#include <memory> 
#include <string> 
#include <string_view> 
#include <array> 
#include <vector> 
#include <stack> 
#include <queue> 
#include <unordered_set> 
#include <set> 
#include <unordered_map> 
#include <map> 
namespace Chessmate {
    using address = std::size_t;
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;
    using string = std::string;
    using string_view = std::string_view;
    template<typename T, address N>
    using Array = std::array<T, N>;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename T>
    using Queue = std::queue<T>;
    template<typename T>
    using Set = std::unordered_set<T>;
    template<typename T>
    using OrderedSet = std::set<T>;
    template<typename Key, typename Value>
    using Map = std::unordered_map<Key, Value>;
    template<typename Key, typename Value>
    using OrderedMap = std::map<Key, Value>;
}
// #include "player.hpp" (HPPMERGE)
namespace Chessmate {
    enum class Player {
        None = 0, White, Black
    };
    Player getEnemy(Player player);
    string fromPlayer(Player player);
    Player toPlayer(const string& str);
    template<typename T>
    struct PlayerValue {
    public:
        PlayerValue(T whitevalue, T blackvalue)
            : m_whitevalue(whitevalue), m_blackvalue(blackvalue) {}
        const T& operator[](Player player) const {
            return (player == Player::White) ? m_whitevalue : m_blackvalue;
        }
    private:
        T m_whitevalue;
        T m_blackvalue;
    };
}
// #include "piece.hpp" (HPPMERGE)
namespace Chessmate {
    enum class PieceType {
        None = 0, Pawn, Knight, Bishop, Rook, Queen, King
    };
    struct Piece {
        Piece();
        Piece(PieceType type, Player player);
        bool isEmpty() const;
        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;
        PieceType type;
        Player player;
    };
    PieceType toPieceType(char ch);
    Piece toPiece(char ch);
    char fromPiece(Piece piece);
}
// #include "square.hpp" (HPPMERGE)
namespace Chessmate {
    using Square = int32;
    using Direction = int32;
    static inline constexpr Square NoSquare = -1;
    bool inBounds(Square square, Direction filedir, Direction rankdir);
    int32 getFile(Square square);
    int32 getRank(Square square);
    Square addSquare(Square square, Direction filedir, Direction rankdir);
    string fromSquare(Square square);
    Square toSquare(const string& str);
}
// #include "move.hpp" (HPPMERGE)
namespace Chessmate {
    enum class MoveFlag {
        None = 0, Normal, DoubleAdvance, EnPassant, CastleK, CastleQ, PromoteN, PromoteB, PromoteR, PromoteQ
    };
    struct Move {
        Move();
        Move(MoveFlag flag);
        Move(Piece piece, Square origin, Square target);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir);
        Move(Piece piece, Square origin, Square target, MoveFlag flag);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir, MoveFlag flag);
        bool isValid() const;
        bool operator==(const Move& other) const;
        bool operator!=(const Move& other) const;
        string toAlgebraicNotation() const;
        Piece piece;
        Square origin;
        Square target;
        MoveFlag flag;
    };
}
// #include "castle.hpp" (HPPMERGE)
namespace Chessmate {
    enum class Castle {
        WhiteKingside, WhiteQueenside, BlackKingside, BlackQueenside
    };
    struct CastleState {
    public:
        CastleState();
        CastleState(bool wk, bool wq, bool bk, bool bq);
        bool operator[](Castle type) const;
        bool& operator[](Castle type);
        bool operator==(const CastleState& other) const;
        bool operator!=(const CastleState& other) const;
    private:
        bool m_wk, m_wq, m_bk, m_bq;
    };
}
// #include "board.hpp" (HPPMERGE)
namespace Chessmate {
    class Board {
    public:
        Board(const string& fen);
        Board(const Board& oldboard, const Move& move);
        void fromFEN(const string& fen);
        string toFEN() const;
        Move fromAlgebraicNotation(string notation) const;
        const Piece& get(Square square) const;
        const Piece& get(Square square, Direction filedir, Direction rankdir) const;
        void set(Square square, Piece piece);
        Player player(Square square) const;
        PieceType type(Square square) const;
        bool isEmpty(Square square) const;
        bool isEmpty(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEmpty(Square square, Direction filedir, Direction rankdir) const;
        bool isFriendly(Square square) const;
        bool isFriendly(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndFriendly(Square square, Direction filedir, Direction rankdir) const;
        bool isEnemy(Square square) const;
        bool isEnemy(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEnemy(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEquals(Square square, Direction filedir, Direction rankdir, Piece piece) const;
        List<Move> getMoveset() const;
        List<Move> getLegalMoves() const;
        Square findPiece(Piece piece) const;
        List<Square> getAttackingSquares(Square square, Player player) const;
        bool isSquareAttackedBy(Square square, Player player) const;
        bool canCaptureKing() const;
        bool isKingAttacked() const;
        bool inCheck() const;
        bool inCheckmate() const;
        bool isDraw() const;
        bool hasSamePositions(const Board& other) const;
        string toString() const;
        auto begin();
        const auto begin() const;
        auto end();
        const auto end() const;
        Player active;
        CastleState castle;
        Square enpassant;
        uint32 halfmove, fullmove;
        Array<Piece, 64> squares;
    };
}
// #include "gameserver.hpp" (HPPMERGE)
namespace Chessmate {
    string createGame();
    string joinGame(int32 gameid, Player player);
    string doMove(int32 gameid, int32 playerid, string movestring);
    string getGameState(int32 gameid, int32 depth = 0);
}
// #include "chessmate.hpp" (HPPMERGE)
#include "bot/minimax.hpp"