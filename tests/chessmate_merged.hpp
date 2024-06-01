#pragma once
// #include <...> (HPPMERGE)
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

// #include "common.hpp" (HPPMERGE)
namespace Chessmate {
    // Types
    // Types :: address
    using address = std::size_t;
    // Types :: int
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    // Types :: uint
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;
    // Types :: string
    using string = std::string;
    // Types :: string_view
    using string_view = std::string_view;
    // Structures
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
    // Player
    enum class Player {
        None = 0, White, Black
    };
    // Enemy
    Player getEnemy(Player player);
    // FromPlayer / ToPlayer
    string fromPlayer(Player player);
    Player toPlayer(const string& str);
    // PlayerValue
    template<typename T>
    struct PlayerValue {
    public:
        // Constructor
        PlayerValue(T whitevalue, T blackvalue)
            : m_whitevalue(whitevalue), m_blackvalue(blackvalue) {}
        // Operator[]
        const T& operator[](Player player) const {
            return (player == Player::White) ? m_whitevalue : m_blackvalue;
        }
    private:
        // Member
        T m_whitevalue;
        T m_blackvalue;
    };
}

// #include "piece.hpp" (HPPMERGE)
namespace Chessmate {
    // PieceType
    enum class PieceType {
        None = 0, Pawn, Knight, Bishop, Rook, Queen, King
    };
    // Piece
    struct Piece {
        // Constructor
        Piece();
        Piece(PieceType type, Player player);
        // IsEmpty
        bool isEmpty() const;
        // Compare
        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;
        // Member
        PieceType type;
        Player player;
    };
    // ToPieceType
    PieceType toPieceType(char ch);
    // ToPiece / FromPiece
    Piece toPiece(char ch);
    char fromPiece(Piece piece);
}

// #include "square.hpp" (HPPMERGE)
namespace Chessmate {
    // Square / Direction
    using Square = int32;
    using Direction = int32;
    // NoSquare
    static inline constexpr Square NoSquare = -1;
    // InBounds
    bool inBounds(Square square, Direction filedir, Direction rankdir);
    // File / Rank
    int32 getFile(Square square);
    int32 getRank(Square square);
    // AddSquare
    Square addSquare(Square square, Direction filedir, Direction rankdir);
    // FromSquare / ToSquare
    string fromSquare(Square square);
    Square toSquare(const string& str);
}

// #include "move.hpp" (HPPMERGE)
namespace Chessmate {
    // MoveFlag
    enum class MoveFlag {
        None = 0, Normal, DoubleAdvance, EnPassant, CastleK, CastleQ, PromoteN, PromoteB, PromoteR, PromoteQ
    };
    // Move
    struct Move {
        // Constructor
        Move();
        Move(MoveFlag flag);
        Move(Piece piece, Square origin, Square target);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir);
        Move(Piece piece, Square origin, Square target, MoveFlag flag);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir, MoveFlag flag);
        // Valid
        bool isValid() const;
        // Compare
        bool operator==(const Move& other) const;
        bool operator!=(const Move& other) const;
        // Algebraic Notation (UCI Standard)
        string toAlgebraicNotation() const;
        // Member
        Piece piece;
        Square origin;
        Square target;
        MoveFlag flag;
    };
}

// #include "castle.hpp" (HPPMERGE)
namespace Chessmate {
    // Castle
    enum class Castle {
        WhiteKingside, WhiteQueenside, BlackKingside, BlackQueenside
    };
    // CastleState
    struct CastleState {
    public:
        // Constructor
        CastleState();
        CastleState(bool wk, bool wq, bool bk, bool bq);
        // Operator[]
        bool operator[](Castle type) const;
        bool& operator[](Castle type);
        // Compare
        bool operator==(const CastleState& other) const;
        bool operator!=(const CastleState& other) const;
    private:
        // Member
        bool m_wk, m_wq, m_bk, m_bq;
    };
}

// #include "board.hpp" (HPPMERGE)
namespace Chessmate {
    // Board
    class Board {
    public:
        // Constructor
        Board(const string& fen);
        Board(const Board& oldboard, const Move& move);
        // FEN
        void fromFEN(const string& fen);
        string toFEN() const;
        // Algebraic Notation (UCI Standard)
        Move fromAlgebraicNotation(string notation) const;
        // Square
        // Square :: Get / Set
        const Piece& get(Square square) const;
        const Piece& get(Square square, Direction filedir, Direction rankdir) const;
        void set(Square square, Piece piece);
        // Square :: Player / Type
        Player player(Square square) const;
        PieceType type(Square square) const;
        // Square :: IsEmpty
        bool isEmpty(Square square) const;
        bool isEmpty(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEmpty(Square square, Direction filedir, Direction rankdir) const;
        // Square :: IsFriendly
        bool isFriendly(Square square) const;
        bool isFriendly(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndFriendly(Square square, Direction filedir, Direction rankdir) const;
        // Square :: IsEnemy
        bool isEnemy(Square square) const;
        bool isEnemy(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEnemy(Square square, Direction filedir, Direction rankdir) const;
        // Square :: InBoundsAndEquals
        bool inBoundsAndEquals(Square square, Direction filedir, Direction rankdir, Piece piece) const;
        // Move
        // Move :: Moveset
        List<Move> getMoveset() const;
        // Move :: LegalMoves
        List<Move> getLegalMoves() const;
        // Check
        // Check :: FindPiece
        Square findPiece(Piece piece) const;
        // Check :: AttackingSquares
        List<Square> getAttackingSquares(Square square, Player player) const;
        bool isSquareAttackedBy(Square square, Player player) const;
        // Check :: CanCaptureKing / IsKingAttacked
        bool canCaptureKing() const;
        bool isKingAttacked() const;
        // Check :: Check / Checkmate
        bool inCheck() const;
        bool inCheckmate() const;
        // Draw
        // Draw :: isDraw
        bool isDraw() const;
        // Draw :: HasSamePositions (According to FIDE)
        bool hasSamePositions(const Board& other) const;
        // String
        string toString() const;
        // Iterate
        auto begin();
        const auto begin() const;
        auto end();
        const auto end() const;
        // Member
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

// #include "minimax.hpp" (HPPMERGE)
namespace Chessmate {
    // MinimaxResult
    struct MinimaxResult {
        // Constructor
        MinimaxResult(Move move, float eval);
        // Compare
        bool operator<(const MinimaxResult& other) const;
        bool operator<=(const MinimaxResult& other) const;
        bool operator>(const MinimaxResult& other) const;
        bool operator>=(const MinimaxResult& other) const;
        // Member
        Move move;
        float eval;
    };
    // Minimize / Maximize
    MinimaxResult minimize(const Board& board, uint32 depth, float alpha, float beta);
    MinimaxResult maximize(const Board& board, uint32 depth, float alpha, float beta);
    // Minimax
    MinimaxResult minimax(const Board& board, uint32 depth);
}